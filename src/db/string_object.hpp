#ifndef LIBCACHE_SRC_DB_STRING_OBJECT_HPP_
#define LIBCACHE_SRC_DB_STRING_OBJECT_HPP_

#include <string>

#include "db.hpp"

namespace libcache::db {

class StringObject : public Object {
 public:
  explicit StringObject(std::string value) : value_(std::move(value)) {}

  [[nodiscard]] Type type() const override { return Type::kString; }

  [[nodiscard]] const std::string& value() const { return value_; }
  void set_value(std::string value) { value_ = std::move(value); }

 private:
  std::string value_;
};

}  // namespace libcache::db

#endif  // LIBCACHE_SRC_DB_STRING_OBJECT_HPP_
