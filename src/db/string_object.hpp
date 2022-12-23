#ifndef LIBCACHE_SRC_DB_STRING_OBJECT_HPP_
#define LIBCACHE_SRC_DB_STRING_OBJECT_HPP_

#include <cstdint>
#include <string>
#include <variant>

#include "db.hpp"

namespace libcache::db {

class StringObject : public Object {
 public:
  explicit StringObject(std::string key, ExpireHelper expire_helper,
                        const std::string& value)
      : Object(std::move(key), std::move(expire_helper)) {
    Update(value);
  }

  Type type() const override { return Type::kString; }
  Encoding encoding() const override;

  const std::string& raw() const { return std::get<std::string>(value_); }
  std::string& mut_raw() { return *std::get_if<std::string>(&value_); }
  int64_t int64() const { return std::get<int64_t>(value_); }
  std::string str() const { return IsInt() ? std::to_string(int64()) : raw(); }

  bool IsRaw() const { return std::get_if<std::string>(&value_); }
  bool IsInt() const { return std::get_if<int64_t>(&value_); }

  void Update(const std::string& value);
  std::string Serialize() override;

 private:
  std::variant<std::string, int64_t> value_;
};

}  // namespace libcache::db

#endif  // LIBCACHE_SRC_DB_STRING_OBJECT_HPP_
