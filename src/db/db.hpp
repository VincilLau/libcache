#ifndef LIBCACHE_SRC_DB_DB_HPP_
#define LIBCACHE_SRC_DB_DB_HPP_

#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>

#include "expire/time_wheel.hpp"
#include "libcache/libcache.hpp"
#include "object.hpp"

namespace libcache::db {

class DB {
 public:
  explicit DB(const DBOptions& options)
      : unix_tw_(options.time_wheel_size), boot_tw_(options.time_wheel_size) {}
  ~DB() { FlushDB(); }

  void CleanUpExpired();

  void DumpSnapshot(Status& status, const std::string& path) const;
  void LoadSnapshot(Status& status, const std::string& path);

  void FlushDB() {
    std::lock_guard<std::mutex> lock(mutex_);
    ClearNoLock();
  }
  std::optional<Encoding> ObjectEncoding(const Key& key) const;
  std::optional<Integer> ObjectIdleTime(const Key& key) const;
  Integer Persist(const Key& key) const;
  Integer PExpire(Status& status, const Key& key, int64_t milliseconds,
                  uint64_t flags);
  Integer PExpireAt(Status& status, const Key& key,
                    int64_t unix_time_milliseconds, uint64_t flags);
  Integer PExpireTime(const Key& key) const;
  Integer Pttl(const Key& key) const;
  Integer Touch(const std::vector<Key>& keys);
  enum Type Type(const Key& key) const;

  std::optional<std::string> Get(Status& status, const Key& key) const;
  std::optional<std::string> Set(Status& status, const Key& key,
                                 const std::string& value, uint64_t flags,
                                 const Expiration& expiration);

 private:
  Object::ExpireHelper expire_helper();
  void OnExpired(const std::string& key);

  void ClearNoLock();

  Status Parse(const snapshot::Object& obj);

  bool HasObject(const Key& key) const {
    return objects_.find(key) != objects_.end();
  }
  std::shared_ptr<Object> GetObject(const Key& key) const;
  void PutObject(const Key& key, std::shared_ptr<Object> obj) {
    assert(!HasObject(key));
    objects_[key] = obj;
  }
  void DelObject(const Key& key);

  mutable std::mutex mutex_;
  std::unordered_map<Key, std::shared_ptr<Object>> objects_;
  expire::TimeWheel<expire::UnixTime> unix_tw_;
  expire::TimeWheel<expire::BootTime> boot_tw_;
};

}  // namespace libcache::db

#endif  // LIBCACHE_SRC_DB_DB_HPP_
