#include "cache_impl.hpp"

#include "status.hpp"

using libcache::db::DB;
using std::make_unique;
using std::string;

namespace libcache {

Cache* Cache::New(const Options& options) {
  Status status;
  auto result = New(status, options);
  ThrowIfError(status);
  return result;
}

Cache* Cache::New(Status& status, const Options& options) {
  return CacheImpl::New(status, options);
}

static Status CheckOptions(const Options& options) {
  if (options.db_count == 0) {
    return Status{kDBCountIsZero};
  }
  if (options.expire_timer_interval == 0) {
    return Status{kExpireTimerIntervalIsZero};
  }
  if (options.system_time_wheel_bucket_count == 0) {
    return Status{kSystemTimeWheelBucketCountIsZero};
  }
  if (options.steady_time_wheel_bucket_count == 0) {
    return Status{kSteadyTimeWheelBucketCountIsZero};
  }
  return {};
}

CacheImpl* CacheImpl::New(Status& status, const Options& options) {
  status = CheckOptions(options);
  if (status.Error()) {
    return nullptr;
  }
  return new CacheImpl(options);
}

CacheImpl::CacheImpl(const Options& options)
    : dbs_(options.db_count),
      timer(options.expire_timer_interval * 1000,
            [this]() { TimerCallback(); }) {
  for (auto& db : dbs_) {
    db = make_unique<DB>(options);
  }
  timer.Start();
}

void CacheImpl::TimerCallback() {
  for (auto& db : dbs_) {
    db->Tick();
  }
}

void CacheImpl::DumpSnapshot(const string& path) {
  DumpSnapshot(current_db_, path);
}

void CacheImpl::DumpSnapshot(size_t db, const string& path) {
  Status status;
  DumpSnapshot(status, db, path);
  ThrowIfError(status);
}

void CacheImpl::DumpSnapshot(Status& status, const string& path) {
  DumpSnapshot(status, current_db_, path);
}

void CacheImpl::DumpSnapshot(Status& status, size_t db, const string& path) {
  if (db >= dbs_.size()) {
    status = Status{kDBIndexOutOfRange};
    return;
  }
  dbs_[db]->DumpSnapshot(status, path);
}

void CacheImpl::LoadSnapshot(const string& path) {
  LoadSnapshot(current_db_, path);
}

void CacheImpl::LoadSnapshot(size_t db, const string& path) {
  Status status;
  LoadSnapshot(status, db, path);
  ThrowIfError(status);
}

void CacheImpl::LoadSnapshot(Status& status, const string& path) {
  LoadSnapshot(status, current_db_, path);
}

void CacheImpl::LoadSnapshot(Status& status, size_t db, const string& path) {
  if (db >= dbs_.size()) {
    status = Status{kDBIndexOutOfRange};
    return;
  }
  dbs_[db]->LoadSnapshot(status, path);
}

}  // namespace libcache
