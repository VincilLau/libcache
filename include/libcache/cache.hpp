#ifndef LIBCACHE_INCLUDE_LIBCACHE_CACHE_HPP_
#define LIBCACHE_INCLUDE_LIBCACHE_CACHE_HPP_

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "error.hpp"
#include "expiration.hpp"
#include "options.hpp"
#include "types.hpp"

namespace libcache {

class Cache {
 public:
  static Cache* New(const Options& options = {});
  static Cache* New(Status& status, const Options& options = {});

  virtual ~Cache() = default;

  virtual void DumpSnapshot(const std::string& path) = 0;
  virtual void DumpSnapshot(size_t db, const std::string& path) = 0;
  virtual void DumpSnapshot(Status& status, const std::string& path) = 0;
  virtual void DumpSnapshot(Status& status, size_t db,
                            const std::string& path) = 0;

  virtual void LoadSnapshot(const std::string& path) = 0;
  virtual void LoadSnapshot(size_t db, const std::string& path) = 0;
  virtual void LoadSnapshot(Status& status, const std::string& path) = 0;
  virtual void LoadSnapshot(Status& status, size_t db,
                            const std::string& path) = 0;

  // Generic 组
  virtual int64_t Expire(const std::string& key, int64_t seconds,
                         uint64_t flags = 0) = 0;
  virtual int64_t Expire(size_t db, const std::string& key, int64_t seconds,
                         uint64_t flags = 0) = 0;
  virtual int64_t Expire(Status& status, const std::string& key,
                         int64_t seconds, uint64_t flags = 0) = 0;
  virtual int64_t Expire(Status& status, size_t db, const std::string& key,
                         int64_t seconds, uint64_t flags = 0) = 0;

  virtual int64_t ExpireAt(const std::string& key, int64_t unix_time_seconds,
                           uint64_t flags = 0) = 0;
  virtual int64_t ExpireAt(size_t db, const std::string& key,
                           int64_t unix_time_seconds, uint64_t flags = 0) = 0;
  virtual int64_t ExpireAt(Status& status, const std::string& key,
                           int64_t unix_time_seconds, uint64_t flags = 0) = 0;
  virtual int64_t ExpireAt(Status& status, size_t db, const std::string& key,
                           int64_t unix_time_seconds, uint64_t flags = 0) = 0;

  virtual int64_t ExpireTime(const std::string& key) = 0;
  virtual int64_t ExpireTime(size_t db, const std::string& key) = 0;
  virtual int64_t ExpireTime(Status& status, const std::string& key) = 0;
  virtual int64_t ExpireTime(Status& status, size_t db,
                             const std::string& key) = 0;

  virtual std::optional<Encoding> ObjectEncoding(const std::string& key) = 0;
  virtual std::optional<Encoding> ObjectEncoding(size_t db,
                                                 const std::string& key) = 0;
  virtual std::optional<Encoding> ObjectEncoding(Status& status,
                                                 const std::string& key) = 0;
  virtual std::optional<Encoding> ObjectEncoding(Status& status, size_t db,
                                                 const std::string& key) = 0;

  virtual std::optional<int64_t> ObjectIdleTime(const std::string& key) = 0;
  virtual std::optional<int64_t> ObjectIdleTime(size_t db,
                                                const std::string& key) = 0;
  virtual std::optional<int64_t> ObjectIdleTime(Status& status,
                                                const std::string& key) = 0;
  virtual std::optional<int64_t> ObjectIdleTime(Status& status, size_t db,
                                                const std::string& key) = 0;

  virtual int64_t Persist(const std::string& key) = 0;
  virtual int64_t Persist(size_t db, const std::string& key) = 0;
  virtual int64_t Persist(Status& status, const std::string& key) = 0;
  virtual int64_t Persist(Status& status, size_t db,
                          const std::string& key) = 0;

  virtual int64_t PExpire(const std::string& key, int64_t milliseconds,
                          uint64_t flags = 0) = 0;
  virtual int64_t PExpire(size_t db, const std::string& key,
                          int64_t milliseconds, uint64_t flags = 0) = 0;
  virtual int64_t PExpire(Status& status, const std::string& key,
                          int64_t milliseconds, uint64_t flags = 0) = 0;
  virtual int64_t PExpire(Status& status, size_t db, const std::string& key,
                          int64_t milliseconds, uint64_t flags = 0) = 0;

  virtual int64_t PExpireAt(const std::string& key,
                            int64_t unix_time_milliseconds,
                            uint64_t flags = 0) = 0;
  virtual int64_t PExpireAt(Status& status, const std::string& key,
                            int64_t unix_time_milliseconds,
                            uint64_t flags = 0) = 0;
  virtual int64_t PExpireAt(size_t db, const std::string& key,
                            int64_t unix_time_milliseconds,
                            uint64_t flags = 0) = 0;
  virtual int64_t PExpireAt(Status& status, size_t db, const std::string& key,
                            int64_t unix_time_milliseconds,
                            uint64_t flags = 0) = 0;

  virtual int64_t PExpireTime(const std::string& key) = 0;
  virtual int64_t PExpireTime(size_t db, const std::string& key) = 0;
  virtual int64_t PExpireTime(Status& status, const std::string& key) = 0;
  virtual int64_t PExpireTime(Status& status, size_t db,
                              const std::string& key) = 0;

  virtual int64_t Pttl(const std::string& key) = 0;
  virtual int64_t Pttl(size_t db, const std::string& key) = 0;
  virtual int64_t Pttl(Status& status, const std::string& key) = 0;
  virtual int64_t Pttl(Status& status, size_t db, const std::string& key) = 0;

  virtual int64_t Touch(const std::vector<std::string>& keys) = 0;
  virtual int64_t Touch(size_t db, const std::vector<std::string>& keys) = 0;
  virtual int64_t Touch(Status& status,
                        const std::vector<std::string>& keys) = 0;
  virtual int64_t Touch(Status& status, size_t db,
                        const std::vector<std::string>& keys) = 0;

  virtual int64_t Ttl(const std::string& key) = 0;
  virtual int64_t Ttl(size_t db, const std::string& key) = 0;
  virtual int64_t Ttl(Status& status, const std::string& key) = 0;
  virtual int64_t Ttl(Status& status, size_t db, const std::string& key) = 0;

  virtual enum Type Type(const std::string& key) = 0;
  virtual enum Type Type(size_t db, const std::string& key) = 0;
  virtual enum Type Type(Status& status, const std::string& key) = 0;
  virtual enum Type Type(Status& status, size_t db, const std::string& key) = 0;

  // String 组
  virtual int64_t Append(const std::string& key, const std::string& value) = 0;
  virtual int64_t Append(size_t db, const std::string& key,
                         const std::string& value) = 0;
  virtual int64_t Append(Status& status, const std::string& key,
                         const std::string& value) = 0;
  virtual int64_t Append(Status& status, size_t db, const std::string& key,
                         const std::string& value) = 0;

  virtual std::optional<std::string> Get(const std::string& key) = 0;
  virtual std::optional<std::string> Get(size_t db, const std::string& key) = 0;
  virtual std::optional<std::string> Get(Status& status,
                                         const std::string& key) = 0;
  virtual std::optional<std::string> Get(Status& status, size_t db,
                                         const std::string& key) = 0;

  virtual std::optional<std::string> Set(
      const std::string& key, const std::string& value, uint64_t flags = 0,
      const Expiration& expiration = NO_EXPIRE) = 0;
  virtual std::optional<std::string> Set(
      size_t db, const std::string& key, const std::string& value,
      uint64_t flags = 0, const Expiration& expiration = NO_EXPIRE) = 0;
  virtual std::optional<std::string> Set(
      Status& status, const std::string& key, const std::string& value,
      uint64_t flags = 0, const Expiration& expiration = NO_EXPIRE) = 0;
  virtual std::optional<std::string> Set(
      Status& status, size_t db, const std::string& key,
      const std::string& value, uint64_t flags = 0,
      const Expiration& expiration = NO_EXPIRE) = 0;
};

}  // namespace libcache

#endif  // LIBCACHE_INCLUDE_LIBCACHE_CACHE_HPP_
