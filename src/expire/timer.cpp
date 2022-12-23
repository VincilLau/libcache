#include "timer.hpp"

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
  until_ = steady_clock::now() + milliseconds(interval_ms_);
  stop_ = false;
  thread_ = thread([this]() { Loop(); });
}

void Timer::Loop() {
  while (1) {
    {
      unique_lock<mutex> lock(mutex_);
      cv_.wait_until(lock, until_, [this]() { return stop_; });
      if (stop_) {
        return;
      }
    }

    callback_();
    until_ += milliseconds(interval_ms_);
  }
}

}  // namespace libcache::expire
