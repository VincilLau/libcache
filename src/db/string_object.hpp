#ifndef LIBCACHE_SRC_DB_STRING_OBJECT_HPP_
#define LIBCACHE_SRC_DB_STRING_OBJECT_HPP_

#include <cstdint>
#include <string>
#include <variant>

#include "db.hpp"

namespace libcache::db {

class StringObject : public Object {
 public:
  StringObject(std::string key, ExpireHelper expire_helper,
               const std::string& value)
      : Object(std::move(key), std::move(expire_helper)) {
    Update(value);
  }
  StringObject(std::string key, ExpireHelper expire_helper, int64_t i64)
      : Object(std::move(key), std::move(expire_helper)) {
    value_ = i64;
  }

  Type type() const override { return Type::kString; }
  Encoding encoding() const override;

  const std::string& raw() const { return std::get<std::string>(value_); }
  std::string& mut_raw() { return *std::get_if<std::string>(&value_); }
  int64_t i64() const { return std::get<int64_t>(value_); }
  void set_i64(int64_t i64) { value_ = i64; }
  std::string str() const { return IsInt() ? std::to_string(i64()) : raw(); }

  bool IsRaw() const { return std::get_if<std::string>(&value_); }
  bool IsInt() const { return std::get_if<int64_t>(&value_); }

  void Update(const std::string& value);
  std::string Serialize() override;

 private:
  std::variant<std::string, int64_t> value_;
};

}  // namespace libcache::db

#endif  // LIBCACHE_SRC_DB_STRING_OBJECT_HPP_
