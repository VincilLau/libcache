#include "libcache.hpp"

namespace libcache {

static const char* kMessages[] = {
    // kOk
    "OK",
    // kDBIndexOutOfRange
    "DB index is out of range",
    // kNxAndXxGtOrLtNotCompatible
    "NX and XX, GT or LT flags at the same time are not compatible",
    // kGtAndLtNotCompatible
    "GT and LT flags at the same time are not compatible",
    // kWrongType
    "WRONGTYPE Operation against a key holding the wrong kind of value",
    // kSyntaxError
    "syntax error",
    // kDBCountIsZero
    "option.db_count can't be zero",
    // kExpireTimerIntervalIsZero
    "option.expire_timer_interval can't be zero",
    // kSystemTimeWheelBucketCountIsZero
    "option.system_time_wheel_bucket_count can't be zero",
    // kSteadyTimeWheelBucketCountIsZero
    "option.steady_time_wheelbucket_count can't be zero",
};

const char* Status::What() const {
  if (code_ < 0 || code_ >= sizeof(kMessages)) {
    return "unknown error";
  }
  return kMessages[code_];
}

}  // namespace libcache
