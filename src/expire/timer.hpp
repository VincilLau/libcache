#ifndef LIBCACHE_SRC_EXPIRE_TIMER_HPP_
#define LIBCACHE_SRC_EXPIRE_TIMER_HPP_

#include <cassert>
#include <chrono>
#include <condition_variable>
#include <cstddef>
#include <functional>
#include <mutex>
#include <thread>

namespace libcache::expire {

class Timer {
 public:
  using Callback = std::function<void()>;

  Timer(size_t interval_ms, Callback callback)
      : interval_ms_(interval_ms), callback_(std::move(callback)) {
    assert(interval_ms_);
    assert(callback_);
  }
  ~Timer();

  void Start();

 private:
  void Loop();

  size_t interval_ms_ = 0;
  Callback callback_;
  std::chrono::steady_clock::time_point until_;
  bool stop_ = true;

  std::thread thread_;
  mutable std::mutex mutex_;
  mutable std::condition_variable cv_;
};

}  // namespace libcache::expire

#endif  // LIBCACHE_SRC_EXPIRE_TIMER_HPP_
