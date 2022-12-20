#include "cache_impl.hpp"

#include "status.hpp"

using libcache::db::DB;
using std::make_unique;

namespace libcache {

Cache* Cache::Create(const Options& options) {
  Status status;
  auto result = Create(status, options);
  ThrowIfError(status);
  return result;
}

Cache* Cache::Create(Status& status, const Options& options) {
  return CacheImpl::Create(status, options);
}

static Status CheckOptions(const Options& options) {
  if (options.db_count == 0) {
    return Status{kDBCountIsZero};
  }
  if (options.expire_timer_interval == 0) {
    return Status{kExpireTimerIntervalIsZero};
  }
  if (options.system_time_wheel_bucket_count == 0) {
    return Status{kSystemTimeWheelBucketCountIsZero};
  }
  if (options.steady_time_wheel_bucket_count == 0) {
    return Status{kSteadyTimeWheelBucketCountIsZero};
  }
  return {};
}

CacheImpl* CacheImpl::Create(Status& status, const Options& options) {
  status = CheckOptions(options);
  if (status.Error()) {
    return nullptr;
  }
  return new CacheImpl(options);
}

CacheImpl::CacheImpl(const Options& options)
    : dbs_(options.db_count),
      timer(options.expire_timer_interval * 1000,
            [this]() { TimerCallback(); }) {
  for (auto& db : dbs_) {
    db = make_unique<DB>(options);
  }
  timer.Start();
}

void CacheImpl::TimerCallback() {
  for (auto& db : dbs_) {
    db->Tick();
  }
}

}  // namespace libcache
