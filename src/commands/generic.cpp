#include "cache_impl.hpp"

using std::optional;
using std::string;
using std::vector;

namespace libcache {

int64_t CacheImpl::Expire(const string& key, int64_t seconds, uint64_t flags) {
  return Expire(current_db_, key, seconds, flags);
}

int64_t CacheImpl::Expire(size_t db, const string& key, int64_t seconds,
                          uint64_t flags) {
  auto status = Status::OK();
  auto result = Expire(status, db, key, seconds, flags);
  status.ThrowIfError();
  return result;
}

int64_t CacheImpl::Expire(Status& status, const string& key, int64_t seconds,
                          uint64_t flags) {
  return Expire(status, current_db_, key, seconds, flags);
}

int64_t CacheImpl::Expire(Status& status, size_t db, const string& key,
                          int64_t seconds, uint64_t flags) {
  return PExpire(status, db, key, seconds * 1000);
}

int64_t CacheImpl::ExpireAt(const string& key, int64_t unix_time_seconds,
                            uint64_t flags) {
  return ExpireAt(current_db_, key, unix_time_seconds, flags);
}

int64_t CacheImpl::ExpireAt(size_t db, const string& key,
                            int64_t unix_time_seconds, uint64_t flags) {
  auto status = Status::OK();
  auto result = ExpireAt(status, db, key, unix_time_seconds, flags);
  status.ThrowIfError();
  return result;
}

int64_t CacheImpl::ExpireAt(Status& status, const string& key,
                            int64_t unix_time_seconds, uint64_t flags) {
  return ExpireAt(status, current_db_, key, unix_time_seconds, flags);
}

int64_t CacheImpl::ExpireAt(Status& status, size_t db, const string& key,
                            int64_t unix_time_seconds, uint64_t flags) {
  return PExpireAt(status, db, key, unix_time_seconds * 1000, flags);
}

int64_t CacheImpl::ExpireTime(const string& key) {
  return ExpireTime(current_db_, key);
}

int64_t CacheImpl::ExpireTime(size_t db, const string& key) {
  auto status = Status::OK();
  auto result = ExpireTime(status, db, key);
  status.ThrowIfError();
  return result;
}

int64_t CacheImpl::ExpireTime(Status& status, const string& key) {
  return ExpireTime(status, current_db_, key);
}

int64_t CacheImpl::ExpireTime(Status& status, size_t db, const string& key) {
  auto result = PExpireTime(status, db, key);
  status.ThrowIfError();
  if (result < 0) {
    return result;
  }
  return result / 1000;
}

optional<Encoding> CacheImpl::ObjectEncoding(const string& key) {
  return ObjectEncoding(current_db_, key);
}

optional<Encoding> CacheImpl::ObjectEncoding(size_t db, const string& key) {
  auto status = Status::OK();
  auto result = ObjectEncoding(status, db, key);
  status.ThrowIfError();
  return result;
}

optional<Encoding> CacheImpl::ObjectEncoding(Status& status,
                                             const string& key) {
  return ObjectEncoding(status, current_db_, key);
}

optional<Encoding> CacheImpl::ObjectEncoding(Status& status, size_t db,
                                             const string& key) {
  if (db >= dbs_.size()) {
    status = Status::DBIndexOutOfRange();
    return {};
  }
  return dbs_[db]->ObjectEncoding(key);
}

optional<int64_t> CacheImpl::ObjectIdleTime(const string& key) {
  return ObjectIdleTime(current_db_, key);
}

optional<int64_t> CacheImpl::ObjectIdleTime(size_t db, const string& key) {
  auto status = Status::OK();
  auto result = ObjectIdleTime(status, db, key);
  status.ThrowIfError();
  return result;
}

optional<int64_t> CacheImpl::ObjectIdleTime(Status& status, const string& key) {
  return ObjectIdleTime(status, current_db_, key);
}

optional<int64_t> CacheImpl::ObjectIdleTime(Status& status, size_t db,
                                            const string& key) {
  if (db >= dbs_.size()) {
    status = Status::DBIndexOutOfRange();
    return INT64_MIN;
  }
  return dbs_[db]->ObjectIdletime(key);
}

int64_t CacheImpl::Persist(const string& key) {
  return Persist(current_db_, key);
}

int64_t CacheImpl::Persist(size_t db, const string& key) {
  auto status = Status::OK();
  auto result = Persist(status, db, key);
  status.ThrowIfError();
  return result;
}

int64_t CacheImpl::Persist(Status& status, const string& key) {
  return Persist(status, current_db_, key);
}

int64_t CacheImpl::Persist(Status& status, size_t db, const string& key) {
  status = Status::OK();
  if (db >= dbs_.size()) {
    status = Status::DBIndexOutOfRange();
    return INT64_MIN;
  }
  return dbs_[db]->Persist(key);
}

int64_t CacheImpl::PExpire(const string& key, int64_t milliseconds,
                           uint64_t flags) {
  return PExpire(current_db_, key, milliseconds, flags);
}

int64_t CacheImpl::PExpire(size_t db, const string& key, int64_t milliseconds,
                           uint64_t flags) {
  auto status = Status::OK();
  auto result = PExpire(status, db, key, milliseconds, flags);
  status.ThrowIfError();
  return result;
}

int64_t CacheImpl::PExpire(Status& status, const string& key,
                           int64_t milliseconds, uint64_t flags) {
  return PExpire(status, current_db_, key, milliseconds, flags);
}

int64_t CacheImpl::PExpire(Status& status, size_t db, const string& key,
                           int64_t milliseconds, uint64_t flags) {
  if (db >= dbs_.size()) {
    status = Status::DBIndexOutOfRange();
    return INT64_MIN;
  }
  return dbs_[db]->PExpire(status, key, milliseconds, flags);
}

int64_t CacheImpl::PExpireAt(const string& key, int64_t unix_time_milliseconds,
                             uint64_t flags) {
  return PExpireAt(current_db_, key, unix_time_milliseconds, flags);
}

int64_t CacheImpl::PExpireAt(size_t db, const string& key,
                             int64_t unix_time_milliseconds, uint64_t flags) {
  auto status = Status::OK();
  auto result = PExpireAt(status, db, key, unix_time_milliseconds, flags);
  status.ThrowIfError();
  return result;
}

int64_t CacheImpl::PExpireAt(Status& status, const string& key,
                             int64_t unix_time_milliseconds, uint64_t flags) {
  return PExpireAt(status, current_db_, key, unix_time_milliseconds, flags);
}

int64_t CacheImpl::PExpireAt(Status& status, size_t db, const string& key,
                             int64_t unix_time_milliseconds, uint64_t flags) {
  if (db >= dbs_.size()) {
    status = Status::DBIndexOutOfRange();
    return INT64_MIN;
  }
  return dbs_[db]->PExpireAt(status, key, unix_time_milliseconds, flags);
}

int64_t CacheImpl::PExpireTime(const string& key) {
  return PExpireTime(current_db_, key);
}

int64_t CacheImpl::PExpireTime(size_t db, const string& key) {
  auto status = Status::OK();
  auto result = PExpireTime(status, db, key);
  status.ThrowIfError();
  return result;
}

int64_t CacheImpl::PExpireTime(Status& status, const string& key) {
  return PExpireTime(status, current_db_, key);
}

int64_t CacheImpl::PExpireTime(Status& status, size_t db, const string& key) {
  status = Status::OK();
  if (db >= dbs_.size()) {
    status = Status::DBIndexOutOfRange();
    return INT64_MIN;
  }
  return dbs_[db]->PExpireTime(key);
}

int64_t CacheImpl::Pttl(const string& key) { return Pttl(current_db_, key); }

int64_t CacheImpl::Pttl(size_t db, const string& key) {
  auto status = Status::OK();
  auto result = Pttl(status, db, key);
  status.ThrowIfError();
  return result;
}

int64_t CacheImpl::Pttl(Status& status, const string& key) {
  return Pttl(status, current_db_, key);
}

int64_t CacheImpl::Pttl(Status& status, size_t db, const string& key) {
  status = Status::OK();
  if (db >= dbs_.size()) {
    status = Status::DBIndexOutOfRange();
    return INT64_MIN;
  }
  return dbs_[db]->Pttl(key);
}

int64_t CacheImpl::Touch(const vector<string>& keys) {
  return Touch(current_db_, keys);
}

int64_t CacheImpl::Touch(size_t db, const vector<string>& keys) {
  auto status = Status::OK();
  auto result = Touch(status, db, keys);
  status.ThrowIfError();
  return result;
}

int64_t CacheImpl::Touch(Status& status, const vector<string>& keys) {
  return Touch(status, current_db_, keys);
}

int64_t CacheImpl::Touch(Status& status, size_t db,
                         const vector<string>& keys) {
  status = Status::OK();
  if (db >= dbs_.size()) {
    status = Status::DBIndexOutOfRange();
    return INT64_MIN;
  }
  return dbs_[db]->Touch(keys);
}

int64_t CacheImpl::Ttl(const string& key) { return Ttl(current_db_, key); }

int64_t CacheImpl::Ttl(size_t db, const string& key) {
  auto status = Status::OK();
  auto result = Ttl(status, db, key);
  status.ThrowIfError();
  return result;
}

int64_t CacheImpl::Ttl(Status& status, const string& key) {
  return Ttl(status, current_db_, key);
}

int64_t CacheImpl::Ttl(Status& status, size_t db, const string& key) {
  auto result = Pttl(status, db, key);
  status.ThrowIfError();
  if (result < 0) {
    return result;
  }
  return result / 1000;
}

enum Type CacheImpl::Type(const string& key) { return Type(current_db_, key); }

enum Type CacheImpl::Type(size_t db, const string& key) {
  auto status = Status::OK();
  auto result = Type(status, current_db_, key);
  status.ThrowIfError();
  return result;
}

enum Type CacheImpl::Type(Status& status, const string& key) {
  return Type(status, current_db_, key);
}

enum Type CacheImpl::Type(Status& status, size_t db, const string& key) {
  status = Status::OK();
  if (db >= dbs_.size()) {
    status = Status::DBIndexOutOfRange();
    return Type::kNone;
  }
  return dbs_[db]->Type(key);
}

}  // namespace libcache
