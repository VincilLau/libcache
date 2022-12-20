#ifndef LIBCACHE_SRC_EXPIRE_TIME_WHEEL_HPP_
#define LIBCACHE_SRC_EXPIRE_TIME_WHEEL_HPP_

#include <cassert>
#include <functional>
#include <map>
#include <vector>

#include "time_point.hpp"

namespace libcache::expire {

// 时间轮，每 tick 调用一个桶中已过期的回调并清除。
template <typename TP>
class TimeWheel {
 public:
  using Callback = std::function<void()>;

  explicit TimeWheel(size_t bucket_count) : buckets_(bucket_count) {
    assert(bucket_count > 0);
  }

  [[nodiscard]] TP Add(typename TP::ChronoTimePoint expire_at,
                       Callback callback);
  void Remove(const TP& tp);
  void Tick();

 private:
  std::vector<std::map<TP, Callback>> buckets_;
  size_t next_bucket_ = 0;
  size_t next_seq_ = 1;
};

template <typename TP>
inline TP TimeWheel<TP>::Add(typename TP::ChronoTimePoint expire_at,
                             Callback callback) {
  size_t index = std::chrono::duration_cast<TimePoint::Duration>(
                     expire_at.time_since_epoch())
                     .count() %
                 buckets_.size();
  TP tp(std::move(expire_at), next_seq_++);
  buckets_[index].insert({tp, std::move(callback)});
  return tp;
}

template <typename TP>
inline void TimeWheel<TP>::Remove(const TP& tp) {
  size_t index = tp.Msec() % buckets_.size();
  auto n = buckets_[index].erase(tp);
  assert(n);
}

template <typename TP>
inline void TimeWheel<TP>::Tick() {
  auto& bucket = buckets_[next_bucket_];
  next_bucket_ = (next_bucket_ + 1) % buckets_.size();

  while (!bucket.empty()) {
    auto front = bucket.begin();
    const TP& tp = front->first;
    if (tp.Pttl() > 0) {
      break;
    }

    const Callback& callback = front->second;
    callback();
    bucket.erase(front);
  }
}

}  // namespace libcache::expire

#endif  // LIBCACHE_SRC_EXPIRE_TIME_WHEEL_HPP_
