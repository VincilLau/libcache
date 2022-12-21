#include "cache_impl.hpp"
#include "status.hpp"

using std::optional;
using std::string;
using std::vector;

namespace libcache {

int64_t CacheImpl::Expire(const string& key, int64_t seconds, uint64_t flags) {
  return Expire(current_db_, key, seconds, flags);
}

int64_t CacheImpl::Expire(size_t db, const string& key, int64_t seconds,
                          uint64_t flags) {
  Status status;
  auto result = Expire(status, db, key, seconds, flags);
  ThrowIfError(status);
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
  Status status;
  auto result = ExpireAt(status, db, key, unix_time_seconds, flags);
  ThrowIfError(status);
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
  Status status;
  auto result = ExpireTime(status, db, key);
  ThrowIfError(status);
  return result;
}

int64_t CacheImpl::ExpireTime(Status& status, const string& key) {
  return ExpireTime(status, current_db_, key);
}

int64_t CacheImpl::ExpireTime(Status& status, size_t db, const string& key) {
  auto result = PExpireTime(status, db, key);
  ThrowIfError(status);
  if (result < 0) {
    return result;
  }
  return result / 1000;
}

optional<Encoding> CacheImpl::ObjectEncoding(const string& key) {
  return ObjectEncoding(current_db_, key);
}

optional<Encoding> CacheImpl::ObjectEncoding(size_t db, const string& key) {
  Status status;
  auto result = ObjectEncoding(status, db, key);
  ThrowIfError(status);
  return result;
}

optional<Encoding> CacheImpl::ObjectEncoding(Status& status,
                                             const string& key) {
  return ObjectEncoding(status, current_db_, key);
}

optional<Encoding> CacheImpl::ObjectEncoding(Status& status, size_t db,
                                             const string& key) {
  if (db >= dbs_.size()) {
    status = Status{kDBIndexOutOfRange};
    return {};
  }
  return dbs_[db]->ObjectEncoding(status, key);
}

optional<int64_t> CacheImpl::ObjectIdleTime(const string& key) {
  return ObjectIdleTime(current_db_, key);
}

optional<int64_t> CacheImpl::ObjectIdleTime(size_t db, const string& key) {
  Status status;
  auto result = ObjectIdleTime(status, db, key);
  ThrowIfError(status);
  return result;
}

optional<int64_t> CacheImpl::ObjectIdleTime(Status& status, const string& key) {
  return ObjectIdleTime(status, current_db_, key);
}

optional<int64_t> CacheImpl::ObjectIdleTime(Status& status, size_t db,
                                            const string& key) {
  if (db >= dbs_.size()) {
    status = Status{kDBIndexOutOfRange};
    return INT64_MIN;
  }
  return dbs_[db]->ObjectIdleTime(status, key);
}

int64_t CacheImpl::Persist(const string& key) {
  return Persist(current_db_, key);
}

int64_t CacheImpl::Persist(size_t db, const string& key) {
  Status status;
  auto result = Persist(status, db, key);
  ThrowIfError(status);
  return result;
}

int64_t CacheImpl::Persist(Status& status, const string& key) {
  return Persist(status, current_db_, key);
}

int64_t CacheImpl::Persist(Status& status, size_t db, const string& key) {
  if (db >= dbs_.size()) {
    status = Status{kDBIndexOutOfRange};
    return INT64_MIN;
  }
  return dbs_[db]->Persist(status, key);
}

int64_t CacheImpl::PExpire(const string& key, int64_t milliseconds,
                           uint64_t flags) {
  return PExpire(current_db_, key, milliseconds, flags);
}

int64_t CacheImpl::PExpire(size_t db, const string& key, int64_t milliseconds,
                           uint64_t flags) {
  Status status;
  auto result = PExpire(status, db, key, milliseconds, flags);
  ThrowIfError(status);
  return result;
}

int64_t CacheImpl::PExpire(Status& status, const string& key,
                           int64_t milliseconds, uint64_t flags) {
  return PExpire(status, current_db_, key, milliseconds, flags);
}

int64_t CacheImpl::PExpire(Status& status, size_t db, const string& key,
                           int64_t milliseconds, uint64_t flags) {
  if (db >= dbs_.size()) {
    status = Status{kDBIndexOutOfRange};
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
  Status status;
  auto result = PExpireAt(status, db, key, unix_time_milliseconds, flags);
  ThrowIfError(status);
  return result;
}

int64_t CacheImpl::PExpireAt(Status& status, const string& key,
                             int64_t unix_time_milliseconds, uint64_t flags) {
  return PExpireAt(status, current_db_, key, unix_time_milliseconds, flags);
}

int64_t CacheImpl::PExpireAt(Status& status, size_t db, const string& key,
                             int64_t unix_time_milliseconds, uint64_t flags) {
  if (db >= dbs_.size()) {
    status = Status{kDBIndexOutOfRange};
    return INT64_MIN;
  }
  return dbs_[db]->PExpireAt(status, key, unix_time_milliseconds, flags);
}

int64_t CacheImpl::PExpireTime(const string& key) {
  return PExpireTime(current_db_, key);
}

int64_t CacheImpl::PExpireTime(size_t db, const string& key) {
  Status status;
  auto result = PExpireTime(status, db, key);
  ThrowIfError(status);
  return result;
}

int64_t CacheImpl::PExpireTime(Status& status, const string& key) {
  return PExpireTime(status, current_db_, key);
}

int64_t CacheImpl::PExpireTime(Status& status, size_t db, const string& key) {
  if (db >= dbs_.size()) {
    status = Status{kDBIndexOutOfRange};
    return INT64_MIN;
  }
  return dbs_[db]->PExpireTime(status, key);
}

int64_t CacheImpl::Pttl(const string& key) { return Pttl(current_db_, key); }

int64_t CacheImpl::Pttl(size_t db, const string& key) {
  Status status;
  auto result = Pttl(status, db, key);
  ThrowIfError(status);
  return result;
}

int64_t CacheImpl::Pttl(Status& status, const string& key) {
  return Pttl(status, current_db_, key);
}

int64_t CacheImpl::Pttl(Status& status, size_t db, const string& key) {
  if (db >= dbs_.size()) {
    status = Status{kDBIndexOutOfRange};
    return INT64_MIN;
  }
  return dbs_[db]->Pttl(status, key);
}

int64_t CacheImpl::Touch(const vector<string>& keys) {
  return Touch(current_db_, keys);
}

int64_t CacheImpl::Touch(size_t db, const vector<string>& keys) {
  Status status;
  auto result = Touch(status, db, keys);
  ThrowIfError(status);
  return result;
}

int64_t CacheImpl::Touch(Status& status, const vector<string>& keys) {
  return Touch(status, current_db_, keys);
}

int64_t CacheImpl::Touch(Status& status, size_t db,
                         const vector<string>& keys) {
  if (db >= dbs_.size()) {
    status = Status{kDBIndexOutOfRange};
    return INT64_MIN;
  }
  return dbs_[db]->Touch(status, keys);
}

int64_t CacheImpl::Ttl(const string& key) { return Ttl(current_db_, key); }

int64_t CacheImpl::Ttl(size_t db, const string& key) {
  Status status;
  auto result = Ttl(status, db, key);
  ThrowIfError(status);
  return result;
}

int64_t CacheImpl::Ttl(Status& status, const string& key) {
  return Ttl(status, current_db_, key);
}

int64_t CacheImpl::Ttl(Status& status, size_t db, const string& key) {
  auto result = Pttl(status, db, key);
  ThrowIfError(status);
  if (result < 0) {
    return result;
  }
  return result / 1000;
}

enum Type CacheImpl::Type(const string& key) { return Type(current_db_, key); }

enum Type CacheImpl::Type(size_t db, const string& key) {
  Status status;
  auto result = Type(status, current_db_, key);
  ThrowIfError(status);
  return result;
}

enum Type CacheImpl::Type(Status& status, const string& key) {
  return Type(status, current_db_, key);
}

enum Type CacheImpl::Type(Status& status, size_t db, const string& key) {
  if (db >= dbs_.size()) {
    status = Status{kDBIndexOutOfRange};
    return Type::kNone;
  }
  return dbs_[db]->Type(status, key);
}

}  // namespace libcache
