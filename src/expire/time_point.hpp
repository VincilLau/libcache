#ifndef LIBCACHE_SRC_EXPIRE_TIME_POINT_HPP_
#define LIBCACHE_SRC_EXPIRE_TIME_POINT_HPP_

#include <chrono>
#include <cstdint>

namespace libcache::expire {

class UnixTime;
class BootTime;

class TimePoint {
 public:
  using Duration = std::chrono::duration<int64_t, std::milli>;

  enum class Type {
    kUnixTime,
    kBootTime,
  };

  TimePoint(int64_t ms, uint64_t seq) : ms_(ms), seq_(seq) {}

  int64_t ms() const { return ms_; }
  virtual int64_t pttl() const = 0;

  virtual Type type() const = 0;

  virtual int64_t ToUnixTime() const = 0;
  virtual int64_t ToBootTime() const = 0;

 protected:
  uint64_t seq() const { return seq_; }

 private:
  int64_t ms_;
  uint64_t seq_;
};

class UnixTime : public TimePoint {
 public:
  UnixTime(int64_t ms, uint64_t seq) : TimePoint(ms, seq) {}

  bool operator==(const UnixTime& other) const {
    return ms() == other.ms() && seq() == other.seq();
  }
  bool operator<(const UnixTime& other) const;

  Type type() const override { return Type::kUnixTime; }

  int64_t pttl() const override { return ms() - Now(); }

  int64_t ToUnixTime() const override { return ms(); }
  int64_t ToBootTime() const override;

  static int64_t Now();
};

class BootTime : public TimePoint {
 public:
  BootTime(int64_t ms, uint64_t seq) : TimePoint(ms, seq) {}

  bool operator==(const BootTime& other) const {
    return ms() == other.ms() && seq() == other.seq();
  }
  bool operator<(const BootTime& other) const;

  Type type() const override { return Type::kBootTime; }

  int64_t pttl() const override { return ms() - Now(); }

  int64_t ToUnixTime() const override;
  int64_t ToBootTime() const override { return ms(); }

  static int64_t Now();
};

}  // namespace libcache::expire

#endif  // LIBCACHE_SRC_EXPIRE_TIME_POINT_HPP_
