#ifndef LIBCACHE_INCLUDE_LIBCACHE_HPP_
#define LIBCACHE_INCLUDE_LIBCACHE_HPP_

#include <cstdint>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

namespace libcache {

// 命令的选项（标志）。
#define NX (1ULL)
#define XX (1ULL << 1)
#define GT (1ULL << 2)
#define LT (1ULL << 3)
#define KEEPTTL (1ULL << 4)
#define GET (1ULL << 5)

// 过期时间，单位毫秒。
struct Expiration {
  int64_t px;
  int64_t pxat;

  [[nodiscard]] bool operator==(const Expiration& other) const {
    return px == other.px && pxat == other.pxat;
  }

  [[nodiscard]] bool operator!=(const Expiration& other) const {
    return px != other.px || pxat != other.pxat;
  }
};

#define NO_EXPIRE (libcache::Expiration{.px = INT64_MAX, .pxat = INT64_MAX})
#define EX(sec) (libcache::Expiration{.px = (sec)*1000, .pxat = INT64_MAX})
#define EXAT(msec) (libcache::Expiration{.px = INT64_MAX, .pxat = (msec)*1000})
#define PX(sec) (libcache::Expiration{.px = (sec), .pxat = INT64_MAX})
#define PXAT(msec) (libcache::Expiration{.px = INT64_MAX, .pxat = (msec)})

// 错误码。
static constexpr int kOk = 0;
static constexpr int kDBIndexOutOfRange = 1;
static constexpr int kNxAndXxGtOrLtNotCompatible = 2;
static constexpr int kGtAndLtNotCompatible = 3;
static constexpr int kWrongType = 4;
static constexpr int kSyntaxError = 5;
static constexpr int kDBCountIsZero = 6;
static constexpr int kExpireTimerIntervalIsZero = 7;
static constexpr int kSystemTimeWheelBucketCountIsZero = 8;
static constexpr int kSteadyTimeWheelBucketCountIsZero = 9;
static constexpr int kNoSuchFile = 10;
static constexpr int kIOError = 11;
static constexpr int kExpectARegularFile = 12;
static constexpr int kEof = 13;
static constexpr int kCorrupt = 14;

class Exception;

// 命令执行状态。
class Status {
 public:
  Status() = default;
  explicit Status(int code) : code_(code) {}

  [[nodiscard]] int code() const { return code_; }

  [[nodiscard]] bool Ok() const { return code_ == 0; }
  [[nodiscard]] bool Error() const { return code_ != 0; }
  [[nodiscard]] const char* What() const;

  void ThrowIfError() const;

 private:
  int code_ = kOk;
};

// 命令执行出错抛出的异常。
class Exception : public std::exception {
 public:
  Exception(const Status& status) : status_(status) {}

  [[nodiscard]] const Status& status() { return status_; }
  [[nodiscard]] const char* what() const noexcept override {
    return status_.What();
  }

 private:
  Status status_;
};

enum class Type {
  kNone,
  kString,
};

enum class Encoding {
  kRaw,
  kInt,
};

// Cache 的选项。
struct Options {
  size_t db_count = 1;
  size_t expire_timer_interval = 1;
  size_t system_time_wheel_bucket_count = 1;
  size_t steady_time_wheel_bucket_count = 1;
};

class Cache {
 public:
  // 创建一个 Cache 对象。
  // 销毁时直接使用 delete 删除对象。
  [[nodiscard]] static Cache* New(const Options& options = {});
  [[nodiscard]] static Cache* New(Status& status, const Options& options = {});

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

  // Generic 组。
  [[nodiscard]] virtual int64_t Expire(const std::string& key, int64_t seconds,
                                       uint64_t flags = 0) = 0;
  [[nodiscard]] virtual int64_t Expire(size_t db, const std::string& key,
                                       int64_t seconds, uint64_t flags = 0) = 0;
  [[nodiscard]] virtual int64_t Expire(Status& status, const std::string& key,
                                       int64_t seconds, uint64_t flags = 0) = 0;
  [[nodiscard]] virtual int64_t Expire(Status& status, size_t db,
                                       const std::string& key, int64_t seconds,
                                       uint64_t flags = 0) = 0;

  [[nodiscard]] virtual int64_t ExpireAt(const std::string& key,
                                         int64_t unix_time_seconds,
                                         uint64_t flags = 0) = 0;
  [[nodiscard]] virtual int64_t ExpireAt(size_t db, const std::string& key,
                                         int64_t unix_time_seconds,
                                         uint64_t flags = 0) = 0;
  [[nodiscard]] virtual int64_t ExpireAt(Status& status, const std::string& key,
                                         int64_t unix_time_seconds,
                                         uint64_t flags = 0) = 0;
  [[nodiscard]] virtual int64_t ExpireAt(Status& status, size_t db,
                                         const std::string& key,
                                         int64_t unix_time_seconds,
                                         uint64_t flags = 0) = 0;

  [[nodiscard]] virtual int64_t ExpireTime(const std::string& key) = 0;
  [[nodiscard]] virtual int64_t ExpireTime(size_t db,
                                           const std::string& key) = 0;
  [[nodiscard]] virtual int64_t ExpireTime(Status& status,
                                           const std::string& key) = 0;
  [[nodiscard]] virtual int64_t ExpireTime(Status& status, size_t db,
                                           const std::string& key) = 0;

  [[nodiscard]] virtual std::optional<Encoding> ObjectEncoding(
      const std::string& key) = 0;
  [[nodiscard]] virtual std::optional<Encoding> ObjectEncoding(
      size_t db, const std::string& key) = 0;
  [[nodiscard]] virtual std::optional<Encoding> ObjectEncoding(
      Status& status, const std::string& key) = 0;
  [[nodiscard]] virtual std::optional<Encoding> ObjectEncoding(
      Status& status, size_t db, const std::string& key) = 0;

  [[nodiscard]] virtual std::optional<int64_t> ObjectIdleTime(
      const std::string& key) = 0;
  [[nodiscard]] virtual std::optional<int64_t> ObjectIdleTime(
      size_t db, const std::string& key) = 0;
  [[nodiscard]] virtual std::optional<int64_t> ObjectIdleTime(
      Status& status, const std::string& key) = 0;
  [[nodiscard]] virtual std::optional<int64_t> ObjectIdleTime(
      Status& status, size_t db, const std::string& key) = 0;

  [[nodiscard]] virtual int64_t Persist(const std::string& key) = 0;
  [[nodiscard]] virtual int64_t Persist(size_t db, const std::string& key) = 0;
  [[nodiscard]] virtual int64_t Persist(Status& status,
                                        const std::string& key) = 0;
  [[nodiscard]] virtual int64_t Persist(Status& status, size_t db,
                                        const std::string& key) = 0;

  [[nodiscard]] virtual int64_t PExpire(const std::string& key,
                                        int64_t milliseconds,
                                        uint64_t flags = 0) = 0;
  [[nodiscard]] virtual int64_t PExpire(size_t db, const std::string& key,
                                        int64_t milliseconds,
                                        uint64_t flags = 0) = 0;
  [[nodiscard]] virtual int64_t PExpire(Status& status, const std::string& key,
                                        int64_t milliseconds,
                                        uint64_t flags = 0) = 0;
  [[nodiscard]] virtual int64_t PExpire(Status& status, size_t db,
                                        const std::string& key,
                                        int64_t milliseconds,
                                        uint64_t flags = 0) = 0;

  [[nodiscard]] virtual int64_t PExpireAt(const std::string& key,
                                          int64_t unix_time_milliseconds,
                                          uint64_t flags = 0) = 0;
  [[nodiscard]] virtual int64_t PExpireAt(Status& status,
                                          const std::string& key,
                                          int64_t unix_time_milliseconds,
                                          uint64_t flags = 0) = 0;
  [[nodiscard]] virtual int64_t PExpireAt(size_t db, const std::string& key,
                                          int64_t unix_time_milliseconds,
                                          uint64_t flags = 0) = 0;
  [[nodiscard]] virtual int64_t PExpireAt(Status& status, size_t db,
                                          const std::string& key,
                                          int64_t unix_time_milliseconds,
                                          uint64_t flags = 0) = 0;

  [[nodiscard]] virtual int64_t PExpireTime(const std::string& key) = 0;
  [[nodiscard]] virtual int64_t PExpireTime(size_t db,
                                            const std::string& key) = 0;
  [[nodiscard]] virtual int64_t PExpireTime(Status& status,
                                            const std::string& key) = 0;
  [[nodiscard]] virtual int64_t PExpireTime(Status& status, size_t db,
                                            const std::string& key) = 0;

  [[nodiscard]] virtual int64_t Pttl(const std::string& key) = 0;
  [[nodiscard]] virtual int64_t Pttl(size_t db, const std::string& key) = 0;
  [[nodiscard]] virtual int64_t Pttl(Status& status,
                                     const std::string& key) = 0;
  [[nodiscard]] virtual int64_t Pttl(Status& status, size_t db,
                                     const std::string& key) = 0;

  [[nodiscard]] virtual int64_t Touch(const std::vector<std::string>& keys) = 0;
  [[nodiscard]] virtual int64_t Touch(size_t db,
                                      const std::vector<std::string>& keys) = 0;
  [[nodiscard]] virtual int64_t Touch(Status& status,
                                      const std::vector<std::string>& keys) = 0;
  [[nodiscard]] virtual int64_t Touch(Status& status, size_t db,
                                      const std::vector<std::string>& keys) = 0;

  [[nodiscard]] virtual int64_t Ttl(const std::string& key) = 0;
  [[nodiscard]] virtual int64_t Ttl(size_t db, const std::string& key) = 0;
  [[nodiscard]] virtual int64_t Ttl(Status& status, const std::string& key) = 0;
  [[nodiscard]] virtual int64_t Ttl(Status& status, size_t db,
                                    const std::string& key) = 0;

  [[nodiscard]] virtual enum Type Type(const std::string& key) = 0;
  [[nodiscard]] virtual enum Type Type(size_t db, const std::string& key) = 0;
  [[nodiscard]] virtual enum Type Type(Status& status,
                                       const std::string& key) = 0;
  [[nodiscard]] virtual enum Type Type(Status& status, size_t db,
                                       const std::string& key) = 0;

  // String 组。
  [[nodiscard]] virtual std::optional<std::string> Get(
      const std::string& key) = 0;
  [[nodiscard]] virtual std::optional<std::string> Get(
      size_t db, const std::string& key) = 0;
  [[nodiscard]] virtual std::optional<std::string> Get(
      Status& status, const std::string& key) = 0;
  [[nodiscard]] virtual std::optional<std::string> Get(
      Status& status, size_t db, const std::string& key) = 0;

  [[nodiscard]] virtual std::optional<std::string> Set(
      const std::string& key, const std::string& value, uint64_t flags = 0,
      const Expiration& expiration = NO_EXPIRE) = 0;
  [[nodiscard]] virtual std::optional<std::string> Set(
      size_t db, const std::string& key, const std::string& value,
      uint64_t flags = 0, const Expiration& expiration = NO_EXPIRE) = 0;
  [[nodiscard]] virtual std::optional<std::string> Set(
      Status& status, const std::string& key, const std::string& value,
      uint64_t flags = 0, const Expiration& expiration = NO_EXPIRE) = 0;
  [[nodiscard]] virtual std::optional<std::string> Set(
      Status& status, size_t db, const std::string& key,
      const std::string& value, uint64_t flags = 0,
      const Expiration& expiration = NO_EXPIRE) = 0;
};

}  // namespace libcache

#endif  // LIBCACHE_INCLUDE_LIBCACHE_HPP_
