#include "db.hpp"

#include "snapshot/snapshot.hpp"
#include "string_object.hpp"

using libcache::expire::BootTime;
using libcache::expire::TimePoint;
using libcache::expire::UnixTime;
using libcache::snapshot::SnapshotReader;
using libcache::snapshot::SnapshotWriter;
using std::lock_guard;
using std::mutex;
using std::shared_ptr;
using std::string;
using std::unique_ptr;

namespace libcache::db {

Object::ExpireHelper DB::expire_helper() {
  Object::ExpireHelper helper;
  helper.px = [this](const string& key, int64_t ms) -> BootTime {
    int64_t at = ms + BootTime::Now();
    return boot_tw_.Add(at, [this, key = key]() { OnExpired(key); });
  };
  helper.pxat = [this](const string& key, int64_t ms) -> UnixTime {
    return unix_tw_.Add(ms, [this, key = key]() { OnExpired(key); });
  };
  helper.persist = [this](const TimePoint* tp) {
    if (tp->type() == TimePoint::Type::kUnixTime) {
      auto ut = *dynamic_cast<const UnixTime*>(tp);
      unix_tw_.Remove(ut);
    } else {
      auto bt = *dynamic_cast<const BootTime*>(tp);
      boot_tw_.Remove(bt);
    }
  };
  return helper;
}

void DB::CleanUpExpired() {
  lock_guard<mutex> lock(mutex_);
  unix_tw_.Tick();
  boot_tw_.Tick();
}

void DB::DumpSnapshot(Status& status, const string& path) const {
  status = Status::OK();
  lock_guard<mutex> lock(mutex_);

  unique_ptr<SnapshotWriter> writer;
  status = SnapshotWriter::Open(path, writer);
  for (const auto& [key, _] : objects_) {
    auto obj = GetObject(key);
    status = writer->Append(key, obj);
    if (status.error()) {
      return;
    }
  }
}

void DB::LoadSnapshot(Status& status, const string& path) {
  status = Status::OK();
  lock_guard<mutex> lock(mutex_);
  ClearNoLock();

  unique_ptr<SnapshotReader> reader;
  status = SnapshotReader::Open(path, reader);
  if (status.error()) {
    return;
  }

  while (1) {
    shared_ptr<Object> obj;
    status = reader->Read(obj, expire_helper());
    if (status.code() == kEof) {
      return;
    }
    if (status.error()) {
      ClearNoLock();
      return;
    }

    PutObject(obj->key(), obj);
  }
}

void DB::ClearNoLock() {
  for (auto& [_, obj] : objects_) {
    if (obj->HasExpire()) {
      obj->Persist();
    }
  }
  objects_.clear();
}

shared_ptr<Object> DB::GetObject(const string& key) const {
  auto it = objects_.find(key);
  if (it == objects_.end()) {
    return nullptr;
  }

  auto obj = it->second;
  if (!obj->HasExpire()) {
    return obj;
  }

  int64_t pttl = obj->pttl();
  if (pttl <= 0) {
    return nullptr;
  }

  return obj;
}

void DB::DelObject(const string& key) {
  assert(HasObject(key));

  auto obj = objects_.at(key);
  if (obj->HasExpire()) {
    obj->Persist();
  }
  objects_.erase(key);
}

void DB::OnExpired(const string& key) {
  auto it = objects_.find(key);
  assert(it != objects_.end());
  auto obj = it->second;
  assert(obj->HasExpire());
  objects_.erase(it);
}

}  // namespace libcache::db
