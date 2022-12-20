#ifndef LIBCACHE_SRC_DB_OBJECT_HPP_
#define LIBCACHE_SRC_DB_OBJECT_HPP_

#include <cassert>
#include <memory>

#include "expire/time_point.hpp"

namespace libcache::db {

class Object {
 public:
  enum class Type {
    kString,
  };

  ~Object() {
    // 析构 Object 前必须去除过期时间。
    assert(!expire_at_);
  }

  [[nodiscard]] virtual Type type() const = 0;
  [[nodiscard]] bool IsString() const {
    return type() == Object::Type::kString;
  }

  [[nodiscard]] auto expire_at() const { return expire_at_; }
  void set_expire_at(std::shared_ptr<expire::TimePoint> expire_at) {
    expire_at_ = expire_at;
  }

 private:
  std::shared_ptr<expire::TimePoint> expire_at_;
};

}  // namespace libcache::db

#endif  // LIBCACHE_SRC_DB_OBJECT_HPP_
