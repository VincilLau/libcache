#include "timer.hpp"

#include <cassert>
#include <chrono>

using std::chrono::milliseconds;
using std::chrono::steady_clock;

using std::lock_guard;
using std::mutex;
using std::thread;
using std::unique_lock;
using std::chrono::steady_clock;

namespace libcache::expire {

Timer::~Timer() {
  if (stop_) {
    return;
  }

  {
    lock_guard<mutex> lock(mutex_);
    stop_ = true;
  }

  cv_.notify_one();
  thread_.join();
}

void Timer::Start() {
  assert(interval_ > 0);

  next_tick_ = steady_clock::now() + milliseconds(interval_);
  stop_ = false;
  thread_ = thread([this]() { Loop(); });
}

void Timer::Loop() {
  while (1) {
    {
      unique_lock<mutex> lock(mutex_);
      cv_.wait_until(lock, next_tick_, [this]() { return stop_; });
      if (stop_) {
        break;
      }
    }

    callback_();
    next_tick_ += milliseconds(interval_);
  }
}

}  // namespace libcache::expire
