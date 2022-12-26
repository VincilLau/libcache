#include "cache_impl.hpp"

using std::optional;
using std::string;

namespace libcache {

int64_t CacheImpl::Append(const string& key, const string& value) {
  return Append(current_db_, key, value);
}

int64_t CacheImpl::Append(size_t db, const string& key, const string& value) {
  auto status = Status::OK();
  auto result = Append(status, db, key, value);
  status.ThrowIfError();
  return result;
}

int64_t CacheImpl::Append(Status& status, const string& key,
                          const string& value) {
  return Append(status, current_db_, key, value);
}

int64_t CacheImpl::Append(Status& status, size_t db, const string& key,
                          const string& value) {
  if (db >= dbs_.size()) {
    status = Status::DBIndexOutOfRange();
    return {};
  }
  return dbs_[db]->Append(status, key, value);
}

int64_t CacheImpl::Decr(const string& key) { return Decr(current_db_, key); }

int64_t CacheImpl::Decr(size_t db, const string& key) {
  auto status = Status::OK();
  auto result = CacheImpl::Decr(status, db, key);
  status.ThrowIfError();
  return result;
}

int64_t CacheImpl::Decr(Status& status, const string& key) {
  return Decr(status, current_db_, key);
}

int64_t CacheImpl::Decr(Status& status, size_t db, const string& key) {
  return DecrBy(status, db, key, 1);
}

int64_t CacheImpl::DecrBy(const string& key, int64_t decrement) {
  return DecrBy(current_db_, key, decrement);
}

int64_t CacheImpl::DecrBy(size_t db, const string& key, int64_t decrement) {
  auto status = Status::OK();
  auto result = CacheImpl::DecrBy(status, db, key, decrement);
  status.ThrowIfError();
  return result;
}

int64_t CacheImpl::DecrBy(Status& status, const string& key,
                          int64_t decrement) {
  return DecrBy(status, current_db_, key, decrement);
}

int64_t CacheImpl::DecrBy(Status& status, size_t db, const string& key,
                          int64_t decrement) {
  if (db >= dbs_.size()) {
    status = Status::DBIndexOutOfRange();
    return {};
  }
  return dbs_[db]->DecrBy(status, key, decrement);
}

int64_t CacheImpl::Incr(const string& key) { return Incr(current_db_, key); }

int64_t CacheImpl::Incr(size_t db, const string& key) {
  auto status = Status::OK();
  auto result = CacheImpl::Incr(status, db, key);
  status.ThrowIfError();
  return result;
}

int64_t CacheImpl::Incr(Status& status, const string& key) {
  return Incr(status, current_db_, key);
}

int64_t CacheImpl::Incr(Status& status, size_t db, const string& key) {
  return IncrBy(status, db, key, 1);
}

int64_t CacheImpl::IncrBy(const string& key, int64_t decrement) {
  return IncrBy(current_db_, key, decrement);
}

int64_t CacheImpl::IncrBy(size_t db, const string& key, int64_t decrement) {
  auto status = Status::OK();
  auto result = CacheImpl::IncrBy(status, db, key, decrement);
  status.ThrowIfError();
  return result;
}

int64_t CacheImpl::IncrBy(Status& status, const string& key,
                          int64_t decrement) {
  return IncrBy(status, current_db_, key, decrement);
}

int64_t CacheImpl::IncrBy(Status& status, size_t db, const string& key,
                          int64_t increment) {
  if (db >= dbs_.size()) {
    status = Status::DBIndexOutOfRange();
    return {};
  }
  return dbs_[db]->IncrBy(status, key, increment);
}

optional<string> CacheImpl::Get(const string& key) {
  return Get(current_db_, key);
}

optional<string> CacheImpl::Get(size_t db, const string& key) {
  auto status = Status::OK();
  auto result = Get(status, db, key);
  status.ThrowIfError();
  return result;
}

optional<string> CacheImpl::Get(Status& status, const string& key) {
  return Get(status, current_db_, key);
}

optional<string> CacheImpl::Get(Status& status, size_t db, const string& key) {
  if (db >= dbs_.size()) {
    status = Status::DBIndexOutOfRange();
    return {};
  }
  return dbs_[db]->Get(status, key);
}

optional<string> CacheImpl::Set(const string& key, const string& value,
                                uint64_t flags, const Expiration& expiration) {
  return Set(current_db_, key, value, flags, expiration);
}

optional<string> CacheImpl::Set(size_t db, const string& key,
                                const string& value, uint64_t flags,
                                const Expiration& expiration) {
  auto status = Status::OK();
  auto result = Set(status, db, key, value, flags, expiration);
  status.ThrowIfError();
  return result;
}

optional<string> CacheImpl::Set(Status& status, const string& key,
                                const string& value, uint64_t flags,
                                const Expiration& expiration) {
  return Set(status, current_db_, key, value, flags, expiration);
}

optional<string> CacheImpl::Set(Status& status, size_t db, const string& key,
                                const string& value, uint64_t flags,
                                const Expiration& expiration) {
  if (db >= dbs_.size()) {
    status = Status::DBIndexOutOfRange();
    return {};
  }
  return dbs_[db]->Set(status, key, value, flags, expiration);
}

}  // namespace libcache
