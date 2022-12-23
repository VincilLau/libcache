#include "time_point.hpp"

using std::chrono::duration_cast;
using std::chrono::steady_clock;
using std::chrono::system_clock;

namespace libcache::expire {

bool UnixTime::operator<(const UnixTime& other) const {
  if (ms() < other.ms()) {
    return true;
  }
  if (ms() > other.ms()) {
    return false;
  }
  return seq() < other.seq();
}

int64_t UnixTime::ToBootTime() const {
  auto unix_now = system_clock::now();
  auto boot_now = steady_clock::now();
  auto unix_dur = duration_cast<Duration>(unix_now.time_since_epoch());
  auto boot_dur = duration_cast<Duration>(unix_now.time_since_epoch());
  int64_t unix_ms = unix_dur.count();
  int64_t boot_ms = boot_dur.count();
  int64_t boot_time = ms() - unix_ms + boot_ms;
  return boot_time;
}

int64_t UnixTime::Now() {
  auto now = system_clock::now();
  auto dur = duration_cast<Duration>(now.time_since_epoch());
  return dur.count();
}

bool BootTime::operator<(const BootTime& other) const {
  if (ms() < other.ms()) {
    return true;
  }
  if (ms() > other.ms()) {
    return false;
  }
  return seq() < other.seq();
}

int64_t BootTime::ToUnixTime() const {
  auto unix_now = system_clock::now();
  auto boot_now = steady_clock::now();
  auto unix_dur = duration_cast<Duration>(unix_now.time_since_epoch());
  auto boot_dur = duration_cast<Duration>(unix_now.time_since_epoch());
  int64_t unix_ms = unix_dur.count();
  int64_t boot_ms = boot_dur.count();
  int64_t unix_time = ms() - boot_ms + unix_ms;
  return unix_time;
}

int64_t BootTime::Now() {
  auto now = steady_clock::now();
  auto dur = duration_cast<Duration>(now.time_since_epoch());
  return dur.count();
}

}  // namespace libcache::expire
