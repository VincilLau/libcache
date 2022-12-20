#ifndef LIBCACHE_SRC_DB_DB_HPP_
#define LIBCACHE_SRC_DB_DB_HPP_

#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>

#include "expire/time_wheel.hpp"
#include "libcache.hpp"
#include "object.hpp"

namespace libcache::db {

class DB {
 public:
  explicit DB(const Options& options)
      : system_tw_(options.system_time_wheel_bucket_count),
        steady_tw_(options.steady_time_wheel_bucket_count) {}
  ~DB();

  void Tick();

  [[nodiscard]] int64_t Persist(Status& status, const std::string& key);
  [[nodiscard]] int64_t PExpire(Status& status, const std::string& key,
                                int64_t milliseconds, uint64_t flags);
  [[nodiscard]] int64_t PExpireAt(Status& status, const std::string& key,
                                  int64_t unix_time_milliseconds,
                                  uint64_t flags);
  [[nodiscard]] int64_t PExpireTime(Status& status,
                                    const std::string& key) const;
  [[nodiscard]] int64_t Pttl(Status& status, const std::string& key) const;

  [[nodiscard]] std::optional<std::string> Get(Status& status,
                                               const std::string& key) const;
  [[nodiscard]] std::optional<std::string> Set(Status& status,
                                               const std::string& key,
                                               const std::string& value,
                                               uint64_t flags,
                                               const Expiration& expiration);

 private:
  [[nodiscard]] bool HasObject(const std::string& key) const {
    return objects_.find(key) != objects_.end();
  }
  [[nodiscard]] std::shared_ptr<Object> GetObject(const std::string& key) const;
  void PutObject(const std::string& key, std::shared_ptr<Object> obj);
  void DelObject(const std::string& key);

  void ExpireAfterMsec(const std::string& key, int64_t msec);
  void ExpireAtUnixMsec(const std::string& key, int64_t msec);
  void RemoveExpire(std::shared_ptr<Object> obj);
  void OnExpire(const std::string& key);

  mutable std::mutex mutex_;
  std::unordered_map<std::string, std::shared_ptr<Object>> objects_;
  expire::TimeWheel<expire::SystemTimePoint> system_tw_;
  expire::TimeWheel<expire::SteadyTimePoint> steady_tw_;
};

}  // namespace libcache::db

#endif  // LIBCACHE_SRC_DB_DB_HPP_
