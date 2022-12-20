#ifndef LIBCACHE_SRC_EXPIRE_TIMER_HPP_
#define LIBCACHE_SRC_EXPIRE_TIMER_HPP_

#include <chrono>
#include <condition_variable>
#include <cstddef>
#include <functional>
#include <mutex>
#include <thread>

namespace libcache::expire {

// 定时器。
// 创建一个 Timer 对象将会启动一个与之关联的线程，
// 该线程每隔一段时间调用指定的回调函数，
// 因此需要注意采用同步措施避免数据竞争。
// 定时器会在析构时终止，与之关联的线程随之销毁。
// 该定时器不受系统时钟调整的影响。
class Timer {
 public:
  using Callback = std::function<void()>;

  // 构造一个每隔 interval 毫秒超时一次的定时器。
  // 构造的定时器处于停止状态。
  Timer(size_t interval, Callback callback)
      : interval_(interval), callback_(std::move(callback)) {}
  // 停止定时器并销毁 Timer 对象。与之关联的线程也会被终止。
  ~Timer();

  // 启动定时器。
  void Start();

 private:
  void Loop();

  size_t interval_ = 0;
  Callback callback_;
  std::chrono::steady_clock::time_point next_tick_;
  bool stop_ = true;

  std::thread thread_;
  mutable std::mutex mutex_;
  mutable std::condition_variable cv_;
};

}  // namespace libcache::expire

#endif  // LIBCACHE_SRC_EXPIRE_TIMER_HPP_
