#include "cache_impl.hpp"

#include "libcache/error.hpp"

using libcache::db::DB;
using std::make_unique;
using std::string;

namespace libcache {

Cache* Cache::New(const Options& options) {
  auto status = Status::OK();
  auto result = New(status, options);
  status.ThrowIfError();
  return result;
}

Cache* Cache::New(Status& status, const Options& options) {
  return CacheImpl::New(status, options);
}

static Status CheckOptions(const Options& options) {
  if (options.db_options_array.empty()) {
    return Status::InvalidOptions("options.db_options_array can't be empty");
  }
  if (options.timer_interval == 0) {
    return Status::InvalidOptions(
        "options.timer_interval must be greater than zero");
  }

  for (const auto& db_options : options.db_options_array) {
    if (db_options.time_wheel_size == 0) {
      return Status::InvalidOptions(
          "db_options.time_wheel_size must be greater than zero");
    }
  }
  return Status::OK();
}

CacheImpl* CacheImpl::New(Status& status, const Options& options) {
  status = CheckOptions(options);
  if (status.error()) {
    return nullptr;
  }
  return new CacheImpl(options);
}

CacheImpl::CacheImpl(const Options& options)
    : dbs_(options.db_options_array.size()),
      timer(options.timer_interval, [this]() { TimerCallback(); }) {
  for (size_t i = 0; i < dbs_.size(); i++) {
    dbs_[i] = make_unique<DB>(options.db_options_array[i]);
  }
  timer.Start();
}

void CacheImpl::DumpSnapshot(const string& path) {
  DumpSnapshot(current_db_, path);
}

void CacheImpl::DumpSnapshot(size_t db, const string& path) {
  auto status = Status::OK();
  DumpSnapshot(status, db, path);
  status.ThrowIfError();
}

void CacheImpl::DumpSnapshot(Status& status, const string& path) {
  DumpSnapshot(status, current_db_, path);
}

void CacheImpl::DumpSnapshot(Status& status, size_t db, const string& path) {
  if (db >= dbs_.size()) {
    status = Status::DBIndexOutOfRange();
    return;
  }
  dbs_[db]->DumpSnapshot(status, path);
}

void CacheImpl::LoadSnapshot(const string& path) {
  LoadSnapshot(current_db_, path);
}

void CacheImpl::LoadSnapshot(size_t db, const string& path) {
  auto status = Status::OK();
  LoadSnapshot(status, db, path);
  status.ThrowIfError();
}

void CacheImpl::LoadSnapshot(Status& status, const string& path) {
  LoadSnapshot(status, current_db_, path);
}

void CacheImpl::LoadSnapshot(Status& status, size_t db, const string& path) {
  if (db >= dbs_.size()) {
    status = Status::DBIndexOutOfRange();
    return;
  }
  dbs_[db]->LoadSnapshot(status, path);
}

}  // namespace libcache
