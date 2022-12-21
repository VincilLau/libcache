#ifndef LIBCACHE_SRC_DB_OBJECT_HPP_
#define LIBCACHE_SRC_DB_OBJECT_HPP_

#include <cassert>
#include <memory>

#include "expire/time_point.hpp"
#include "libcache.hpp"
#include "snapshot.pb.h"

namespace libcache::db {

class Object {
 public:
  virtual ~Object() {
    // 析构 Object 前必须去除过期时间。
    assert(!expire_at_);
  }

  [[nodiscard]] virtual Type type() const = 0;
  [[nodiscard]] bool IsString() const { return type() == Type::kString; }

  [[nodiscard]] virtual Encoding encoding() const = 0;

  [[nodiscard]] auto expire_at() const { return expire_at_; }
  void set_expire_at(std::shared_ptr<expire::TimePoint> expire_at) {
    expire_at_ = expire_at;
  }

  [[nodiscard]] int64_t access_time() const { return access_time_; }
  void Touch() { access_time_ = expire::SteadyTimePoint::Now(); }

  [[nodiscard]] virtual std::string Serialize(const std::string& key) = 0;

 protected:
  Object() = default;

  [[nodiscard]] snapshot::Object SnapshotObject(const std::string& key) const;

 private:
  std::shared_ptr<expire::TimePoint> expire_at_;
  int64_t access_time_ = expire::SteadyTimePoint::Now();
};

}  // namespace libcache::db

#endif  // LIBCACHE_SRC_DB_OBJECT_HPP_
