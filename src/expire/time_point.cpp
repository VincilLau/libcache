#include "time_point.hpp"

namespace libcache::expire {

bool SystemTimePoint::operator<(const SystemTimePoint& other) const {
  int64_t msec = Msec();
  int64_t other_msec = other.Msec();
  if (msec < other_msec) {
    return true;
  }
  if (msec > other_msec) {
    return false;
  }
  return seq() < other.seq();
}

bool SystemTimePoint::operator>(const SystemTimePoint& other) const {
  int64_t msec = Msec();
  int64_t other_msec = other.Msec();
  if (msec > other_msec) {
    return true;
  }
  if (msec < other_msec) {
    return false;
  }
  return seq() > other.seq();
}

bool SystemTimePoint::operator<=(const SystemTimePoint& other) const {
  int64_t msec = Msec();
  int64_t other_msec = other.Msec();
  if (msec < other_msec) {
    return true;
  }
  if (msec > other_msec) {
    return false;
  }
  return seq() <= other.seq();
}

bool SystemTimePoint::operator>=(const SystemTimePoint& other) const {
  int64_t msec = Msec();
  int64_t other_msec = other.Msec();
  if (msec > other_msec) {
    return true;
  }
  if (msec < other_msec) {
    return false;
  }
  return seq() >= other.seq();
}

bool SteadyTimePoint::operator<(const SteadyTimePoint& other) const {
  int64_t msec = Msec();
  int64_t other_msec = other.Msec();
  if (msec < other_msec) {
    return true;
  }
  if (msec > other_msec) {
    return false;
  }
  return seq() < other.seq();
}

bool SteadyTimePoint::operator>(const SteadyTimePoint& other) const {
  int64_t msec = Msec();
  int64_t other_msec = other.Msec();
  if (msec > other_msec) {
    return true;
  }
  if (msec < other_msec) {
    return false;
  }
  return seq() > other.seq();
}

bool SteadyTimePoint::operator<=(const SteadyTimePoint& other) const {
  int64_t msec = Msec();
  int64_t other_msec = other.Msec();
  if (msec < other_msec) {
    return true;
  }
  if (msec > other_msec) {
    return false;
  }
  return seq() <= other.seq();
}

bool SteadyTimePoint::operator>=(const SteadyTimePoint& other) const {
  int64_t msec = Msec();
  int64_t other_msec = other.Msec();
  if (msec > other_msec) {
    return true;
  }
  if (msec < other_msec) {
    return false;
  }
  return seq() >= other.seq();
}

}  // namespace libcache::expire
