#ifndef LIBCACHE_SRC_CACHE_IMPL_HPP_
#define LIBCACHE_SRC_CACHE_IMPL_HPP_

#include <vector>

#include "db/db.hpp"
#include "expire/timer.hpp"
#include "libcache/libcache.hpp"

namespace libcache {

class CacheImpl : public Cache {
 public:
  static CacheImpl* New(Status& status, const Options& options);

  void DumpSnapshot(const std::string& path) override;
  void DumpSnapshot(size_t db, const std::string& path) override;
  void DumpSnapshot(Status& status, const std::string& path) override;
  void DumpSnapshot(Status& status, size_t db,
                    const std::string& path) override;

  void LoadSnapshot(const std::string& path) override;
  void LoadSnapshot(size_t db, const std::string& path) override;
  void LoadSnapshot(Status& status, const std::string& path) override;
  void LoadSnapshot(Status& status, size_t db,
                    const std::string& path) override;

  // Generic 组。
  int64_t Expire(const std::string& key, int64_t seconds,
                 uint64_t flags = 0) override;
  int64_t Expire(size_t db, const std::string& key, int64_t seconds,
                 uint64_t flags = 0) override;
  int64_t Expire(Status& status, const std::string& key, int64_t seconds,
                 uint64_t flags = 0) override;
  int64_t Expire(Status& status, size_t db, const std::string& key,
                 int64_t seconds, uint64_t flags = 0) override;

  int64_t ExpireAt(const std::string& key, int64_t unix_time_seconds,
                   uint64_t flags = 0) override;
  int64_t ExpireAt(size_t db, const std::string& key, int64_t unix_time_seconds,
                   uint64_t flags = 0) override;
  int64_t ExpireAt(Status& status, const std::string& key,
                   int64_t unix_time_seconds, uint64_t flags = 0) override;
  int64_t ExpireAt(Status& status, size_t db, const std::string& key,
                   int64_t unix_time_seconds, uint64_t flags = 0) override;

  int64_t ExpireTime(const std::string& key) override;
  int64_t ExpireTime(size_t db, const std::string& key) override;
  int64_t ExpireTime(Status& status, const std::string& key) override;
  int64_t ExpireTime(Status& status, size_t db,
                     const std::string& key) override;

  std::optional<Encoding> ObjectEncoding(const std::string& key) override;
  std::optional<Encoding> ObjectEncoding(size_t db,
                                         const std::string& key) override;
  std::optional<Encoding> ObjectEncoding(Status& status,
                                         const std::string& key) override;
  std::optional<Encoding> ObjectEncoding(Status& status, size_t db,
                                         const std::string& key) override;

  std::optional<int64_t> ObjectIdleTime(const std::string& key) override;
  std::optional<int64_t> ObjectIdleTime(size_t db,
                                        const std::string& key) override;
  std::optional<int64_t> ObjectIdleTime(Status& status,
                                        const std::string& key) override;
  std::optional<int64_t> ObjectIdleTime(Status& status, size_t db,
                                        const std::string& key) override;

  int64_t Persist(const std::string& key) override;
  int64_t Persist(size_t db, const std::string& key) override;
  int64_t Persist(Status& status, const std::string& key) override;
  int64_t Persist(Status& status, size_t db, const std::string& key) override;

  int64_t PExpire(const std::string& key, int64_t milliseconds,
                  uint64_t flags = 0) override;
  int64_t PExpire(size_t db, const std::string& key, int64_t milliseconds,
                  uint64_t flags = 0) override;
  int64_t PExpire(Status& status, const std::string& key, int64_t milliseconds,
                  uint64_t flags = 0) override;
  int64_t PExpire(Status& status, size_t db, const std::string& key,
                  int64_t milliseconds, uint64_t flags = 0) override;

  int64_t PExpireAt(const std::string& key, int64_t unix_time_milliseconds,
                    uint64_t flags = 0) override;
  int64_t PExpireAt(Status& status, const std::string& key,
                    int64_t unix_time_milliseconds,
                    uint64_t flags = 0) override;
  int64_t PExpireAt(size_t db, const std::string& key,
                    int64_t unix_time_milliseconds,
                    uint64_t flags = 0) override;
  int64_t PExpireAt(Status& status, size_t db, const std::string& key,
                    int64_t unix_time_milliseconds,
                    uint64_t flags = 0) override;

  int64_t PExpireTime(const std::string& key) override;
  int64_t PExpireTime(size_t db, const std::string& key) override;
  int64_t PExpireTime(Status& status, const std::string& key) override;
  int64_t PExpireTime(Status& status, size_t db,
                      const std::string& key) override;

  int64_t Pttl(const std::string& key) override;
  int64_t Pttl(size_t db, const std::string& key) override;
  int64_t Pttl(Status& status, const std::string& key) override;
  int64_t Pttl(Status& status, size_t db, const std::string& key) override;

  int64_t Touch(const std::vector<std::string>& keys) override;
  int64_t Touch(size_t db, const std::vector<std::string>& keys) override;
  int64_t Touch(Status& status, const std::vector<std::string>& keys) override;
  int64_t Touch(Status& status, size_t db,
                const std::vector<std::string>& keys) override;

  int64_t Ttl(const std::string& key) override;
  int64_t Ttl(size_t db, const std::string& key) override;
  int64_t Ttl(Status& status, const std::string& key) override;
  int64_t Ttl(Status& status, size_t db, const std::string& key) override;

  enum Type Type(const std::string& key) override;
  enum Type Type(size_t db, const std::string& key) override;
  enum Type Type(Status& status, const std::string& key) override;
  enum Type Type(Status& status, size_t db, const std::string& key) override;

  // String 组。
  std::optional<std::string> Get(const std::string& key) override;
  std::optional<std::string> Get(size_t db, const std::string& key) override;
  std::optional<std::string> Get(Status& status,
                                 const std::string& key) override;
  std::optional<std::string> Get(Status& status, size_t db,
                                 const std::string& key) override;

  std::optional<std::string> Set(
      const std::string& key, const std::string& value, uint64_t flags = 0,
      const Expiration& expiration = NO_EXPIRE) override;
  std::optional<std::string> Set(
      size_t db, const std::string& key, const std::string& value,
      uint64_t flags = 0, const Expiration& expiration = NO_EXPIRE) override;
  std::optional<std::string> Set(
      Status& status, const std::string& key, const std::string& value,
      uint64_t flags = 0, const Expiration& expiration = NO_EXPIRE) override;
  std::optional<std::string> Set(
      Status& status, size_t db, const std::string& key,
      const std::string& value, uint64_t flags = 0,
      const Expiration& expiration = NO_EXPIRE) override;

 private:
  CacheImpl(const Options& options);
  void TimerCallback() {
    for (auto& db : dbs_) {
      db->CleanUpExpired();
    }
  }

  std::vector<std::unique_ptr<db::DB>> dbs_;
  size_t current_db_ = 0;
  expire::Timer timer;
};

}  // namespace libcache

#endif  // LIBCACHE_SRC_CACHE_IMPL_HPP_
