#ifndef LIBCACHE_INCLUDE_LIBCACHE_ERROR_HPP_
#define LIBCACHE_INCLUDE_LIBCACHE_ERROR_HPP_

#include <stdexcept>

namespace libcache {

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

}  // namespace libcache

#endif  // LIBCACHE_INCLUDE_LIBCACHE_ERROR_HPP_
