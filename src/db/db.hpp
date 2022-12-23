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
  std::optional<Encoding> ObjectEncoding(const std::string& key) const;
  std::optional<int64_t> ObjectIdleTime(const std::string& key) const;
  int64_t Persist(const std::string& key) const;
  int64_t PExpire(Status& status, const std::string& key, int64_t milliseconds,
                  uint64_t flags);
  int64_t PExpireAt(Status& status, const std::string& key,
                    int64_t unix_time_milliseconds, uint64_t flags);
  int64_t PExpireTime(const std::string& key) const;
  int64_t Pttl(const std::string& key) const;
  int64_t Touch(const std::vector<std::string>& keys);
  enum Type Type(const std::string& key) const;

  int64_t Append(Status& status, const std::string& key,
                 const std::string& value);
  std::optional<std::string> Get(Status& status, const std::string& key) const;
  std::optional<std::string> Set(Status& status, const std::string& key,
                                 const std::string& value, uint64_t flags,
                                 const Expiration& expiration);

 private:
  Object::ExpireHelper expire_helper();
  void OnExpired(const std::string& key);

  void ClearNoLock();

  bool HasObject(const std::string& key) const {
    return objects_.find(key) != objects_.end();
  }
  std::shared_ptr<Object> GetObject(const std::string& key) const;
  void PutObject(const std::string& key, std::shared_ptr<Object> obj) {
    assert(!HasObject(key));
    assert(key == obj->key());
    objects_[key] = obj;
  }
  void DelObject(const std::string& key);

  mutable std::mutex mutex_;
  std::unordered_map<std::string, std::shared_ptr<Object>> objects_;
  expire::TimeWheel<expire::UnixTime> unix_tw_;
  expire::TimeWheel<expire::BootTime> boot_tw_;
};

}  // namespace libcache::db

#endif  // LIBCACHE_SRC_DB_DB_HPP_
