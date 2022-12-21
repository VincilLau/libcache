#ifndef LIBCACHE_SRC_EXPIRE_TIME_POINT_HPP_
#define LIBCACHE_SRC_EXPIRE_TIME_POINT_HPP_

#include <chrono>
#include <cstdint>
#include <utility>

namespace libcache::expire {

class SystemTimePoint;
class SteadyTimePoint;

// 通过时间和序列号标识的时间点
class TimePoint {
 public:
  using Unit = std::chrono::milliseconds;
  using Ratio = std::milli;
  using Duration = std::chrono::duration<int64_t, Ratio>;

  enum class Type {
    kSystem,
    kSteady,
  };

  [[nodiscard]] virtual Type type() const = 0;
  [[nodiscard]] virtual int64_t Msec() const = 0;
  [[nodiscard]] virtual int64_t Pttl() const = 0;

  [[nodiscard]] const SystemTimePoint* ToSystem() const;
  [[nodiscard]] const SteadyTimePoint* ToSteady() const;

 protected:
  explicit TimePoint(uint64_t seq) : seq_(seq) {}
  [[nodiscard]] uint64_t seq() const { return seq_; }

 private:
  uint64_t seq_;
};

// std::chrono::system_clock 时间点。
class SystemTimePoint : public TimePoint {
 public:
  using Clock = std::chrono::system_clock;
  using ChronoTimePoint = std::chrono::time_point<Clock, Duration>;

  SystemTimePoint(ChronoTimePoint ctp, uint64_t seq)
      : TimePoint(seq), ctp_(std::move(ctp)) {}

  [[nodiscard]] bool operator==(const SystemTimePoint& other) const {
    return seq() == other.seq();
  }

  [[nodiscard]] bool operator!=(const SystemTimePoint& other) const {
    return seq() != other.seq();
  }

  [[nodiscard]] bool operator<(const SystemTimePoint& other) const;
  [[nodiscard]] bool operator>(const SystemTimePoint& other) const;
  [[nodiscard]] bool operator<=(const SystemTimePoint& other) const;
  [[nodiscard]] bool operator>=(const SystemTimePoint& other) const;

  [[nodiscard]] Type type() const override { return Type::kSystem; }
  [[nodiscard]] int64_t Msec() const override {
    return std::chrono::duration_cast<Unit>(ctp_.time_since_epoch()).count();
  }
  [[nodiscard]] int64_t Pttl() const override {
    return Msec() -
           std::chrono::duration_cast<Unit>(Clock::now().time_since_epoch())
               .count();
  }

  [[nodiscard]] static int64_t Now() {
    auto now = std::chrono::time_point_cast<Duration>(Clock::now());
    return std::chrono::duration_cast<Duration>(now.time_since_epoch()).count();
  }

  [[nodiscard]] static ChronoTimePoint FromUnixMsec(int64_t unix_msec) {
    auto now = std::chrono::time_point_cast<Duration>(Clock::now());
    int64_t now_msec =
        std::chrono::duration_cast<Duration>(now.time_since_epoch()).count();
    return now + Unit(unix_msec - now_msec);
  }

 private:
  ChronoTimePoint ctp_;
};

// std::chrono::steady_clock 时间点。
class SteadyTimePoint : public TimePoint {
 public:
  using Clock = std::chrono::steady_clock;
  using ChronoTimePoint = std::chrono::time_point<Clock, Duration>;

  SteadyTimePoint(ChronoTimePoint ctp, uint64_t seq)
      : TimePoint(seq), ctp_(std::move(ctp)) {}

  [[nodiscard]] bool operator==(const SteadyTimePoint& other) const {
    return seq() == other.seq();
  }

  [[nodiscard]] bool operator!=(const SteadyTimePoint& other) const {
    return seq() != other.seq();
  }

  [[nodiscard]] bool operator<(const SteadyTimePoint& other) const;
  [[nodiscard]] bool operator>(const SteadyTimePoint& other) const;
  [[nodiscard]] bool operator<=(const SteadyTimePoint& other) const;
  [[nodiscard]] bool operator>=(const SteadyTimePoint& other) const;

  [[nodiscard]] Type type() const override { return Type::kSteady; }
  [[nodiscard]] int64_t Msec() const override {
    return std::chrono::duration_cast<Unit>(ctp_.time_since_epoch()).count();
  }
  [[nodiscard]] int64_t Pttl() const override {
    return Msec() -
           std::chrono::duration_cast<Unit>(Clock::now().time_since_epoch())
               .count();
  }

  [[nodiscard]] static int64_t Now() {
    auto now = std::chrono::time_point_cast<Duration>(Clock::now());
    return std::chrono::duration_cast<Duration>(now.time_since_epoch()).count();
  }

  [[nodiscard]] static ChronoTimePoint AfterNowSec(int64_t sec) {
    auto now = Clock::now();
    return std::chrono::time_point_cast<Duration>(now) +
           std::chrono::seconds(sec);
  }

  [[nodiscard]] static ChronoTimePoint AfterNowMsec(int64_t msec) {
    auto now = Clock::now();
    return std::chrono::time_point_cast<Duration>(now) +
           std::chrono::milliseconds(msec);
  }

 private:
  ChronoTimePoint ctp_;
};

inline const SystemTimePoint* TimePoint::ToSystem() const {
  return dynamic_cast<const SystemTimePoint*>(this);
}

inline const SteadyTimePoint* TimePoint::ToSteady() const {
  return dynamic_cast<const SteadyTimePoint*>(this);
}

}  // namespace libcache::expire

#endif  // LIBCACHE_SRC_EXPIRE_TIME_POINT_HPP_
