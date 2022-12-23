#include "cache_impl.hpp"

using std::optional;
using std::string;

namespace libcache {

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
