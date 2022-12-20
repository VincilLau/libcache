#ifndef LIBCACHE_SRC_CACHE_IMPL_HPP_
#define LIBCACHE_SRC_CACHE_IMPL_HPP_

#include <vector>

#include "db/db.hpp"
#include "expire/timer.hpp"
#include "libcache.hpp"

namespace libcache {

class CacheImpl : public Cache {
 public:
  [[nodiscard]] static CacheImpl* Create(Status& status,
                                         const Options& options);

  // Generic 组。
  [[nodiscard]] int64_t Expire(const std::string& key, int64_t seconds,
                               uint64_t flags = 0) override;
  [[nodiscard]] int64_t Expire(size_t db, const std::string& key,
                               int64_t seconds, uint64_t flags = 0) override;
  [[nodiscard]] int64_t Expire(Status& status, const std::string& key,
                               int64_t seconds, uint64_t flags = 0) override;
  [[nodiscard]] int64_t Expire(Status& status, size_t db,
                               const std::string& key, int64_t seconds,
                               uint64_t flags = 0) override;

  [[nodiscard]] int64_t ExpireAt(const std::string& key,
                                 int64_t unix_time_seconds,
                                 uint64_t flags = 0) override;
  [[nodiscard]] int64_t ExpireAt(size_t db, const std::string& key,
                                 int64_t unix_time_seconds,
                                 uint64_t flags = 0) override;
  [[nodiscard]] int64_t ExpireAt(Status& status, const std::string& key,
                                 int64_t unix_time_seconds,
                                 uint64_t flags = 0) override;
  [[nodiscard]] int64_t ExpireAt(Status& status, size_t db,
                                 const std::string& key,
                                 int64_t unix_time_seconds,
                                 uint64_t flags = 0) override;

  [[nodiscard]] int64_t ExpireTime(const std::string& key) override;
  [[nodiscard]] int64_t ExpireTime(size_t db, const std::string& key) override;
  [[nodiscard]] int64_t ExpireTime(Status& status,
                                   const std::string& key) override;
  [[nodiscard]] int64_t ExpireTime(Status& status, size_t db,
                                   const std::string& key) override;

  [[nodiscard]] int64_t Persist(const std::string& key) override;
  [[nodiscard]] int64_t Persist(size_t db, const std::string& key) override;
  [[nodiscard]] int64_t Persist(Status& status,
                                const std::string& key) override;
  [[nodiscard]] int64_t Persist(Status& status, size_t db,
                                const std::string& key) override;

  [[nodiscard]] int64_t PExpire(const std::string& key, int64_t milliseconds,
                                uint64_t flags = 0) override;
  [[nodiscard]] int64_t PExpire(size_t db, const std::string& key,
                                int64_t milliseconds,
                                uint64_t flags = 0) override;
  [[nodiscard]] int64_t PExpire(Status& status, const std::string& key,
                                int64_t milliseconds,
                                uint64_t flags = 0) override;
  [[nodiscard]] int64_t PExpire(Status& status, size_t db,
                                const std::string& key, int64_t milliseconds,
                                uint64_t flags = 0) override;

  [[nodiscard]] int64_t PExpireAt(const std::string& key,
                                  int64_t unix_time_milliseconds,
                                  uint64_t flags = 0) override;
  [[nodiscard]] int64_t PExpireAt(Status& status, const std::string& key,
                                  int64_t unix_time_milliseconds,
                                  uint64_t flags = 0) override;
  [[nodiscard]] int64_t PExpireAt(size_t db, const std::string& key,
                                  int64_t unix_time_milliseconds,
                                  uint64_t flags = 0) override;
  [[nodiscard]] int64_t PExpireAt(Status& status, size_t db,
                                  const std::string& key,
                                  int64_t unix_time_milliseconds,
                                  uint64_t flags = 0) override;

  [[nodiscard]] int64_t PExpireTime(const std::string& key) override;
  [[nodiscard]] int64_t PExpireTime(size_t db, const std::string& key) override;
  [[nodiscard]] int64_t PExpireTime(Status& status,
                                    const std::string& key) override;
  [[nodiscard]] int64_t PExpireTime(Status& status, size_t db,
                                    const std::string& key) override;

  [[nodiscard]] int64_t Pttl(const std::string& key) override;
  [[nodiscard]] int64_t Pttl(size_t db, const std::string& key) override;
  [[nodiscard]] int64_t Pttl(Status& status, const std::string& key) override;
  [[nodiscard]] int64_t Pttl(Status& status, size_t db,
                             const std::string& key) override;

  [[nodiscard]] int64_t Ttl(const std::string& key) override;
  [[nodiscard]] int64_t Ttl(size_t db, const std::string& key) override;
  [[nodiscard]] int64_t Ttl(Status& status, const std::string& key) override;
  [[nodiscard]] int64_t Ttl(Status& status, size_t db,
                            const std::string& key) override;

  // String 组。
  [[nodiscard]] std::optional<std::string> Get(const std::string& key) override;
  [[nodiscard]] std::optional<std::string> Get(size_t db,
                                               const std::string& key) override;
  [[nodiscard]] std::optional<std::string> Get(Status& status,
                                               const std::string& key) override;
  [[nodiscard]] std::optional<std::string> Get(Status& status, size_t db,
                                               const std::string& key) override;

  [[nodiscard]] std::optional<std::string> Set(
      const std::string& key, const std::string& value, uint64_t flags = 0,
      const Expiration& expiration = NO_EXPIRE) override;
  [[nodiscard]] std::optional<std::string> Set(
      size_t db, const std::string& key, const std::string& value,
      uint64_t flags = 0, const Expiration& expiration = NO_EXPIRE) override;
  [[nodiscard]] std::optional<std::string> Set(
      Status& status, const std::string& key, const std::string& value,
      uint64_t flags = 0, const Expiration& expiration = NO_EXPIRE) override;
  [[nodiscard]] std::optional<std::string> Set(
      Status& status, size_t db, const std::string& key,
      const std::string& value, uint64_t flags = 0,
      const Expiration& expiration = NO_EXPIRE) override;

 private:
  CacheImpl(const Options& options);
  void TimerCallback();

  std::vector<std::unique_ptr<db::DB>> dbs_;
  size_t current_db_ = 0;
  expire::Timer timer;
};

}  // namespace libcache

#endif  // LIBCACHE_SRC_CACHE_IMPL_HPP_
