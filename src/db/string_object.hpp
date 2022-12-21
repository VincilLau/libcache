#ifndef LIBCACHE_SRC_DB_STRING_OBJECT_HPP_
#define LIBCACHE_SRC_DB_STRING_OBJECT_HPP_

#include <cstdint>
#include <string>
#include <variant>

#include "db.hpp"

namespace libcache::db {

class StringObject : public Object {
 public:
  explicit StringObject(const std::string& value) { Update(value); }

  [[nodiscard]] Type type() const override { return Type::kString; }
  [[nodiscard]] Encoding encoding() const override;

  [[nodiscard]] bool IsRaw() const { return std::get_if<std::string>(&value_); }
  [[nodiscard]] bool IsInt() const { return std::get_if<int64_t>(&value_); }

  [[nodiscard]] const std::string& Raw() const {
    return std::get<std::string>(value_);
  }
  [[nodiscard]] int64_t Int() const { return std::get<int64_t>(value_); }
  [[nodiscard]] std::string String() const {
    return IsInt() ? std::to_string(Int()) : Raw();
  }

  void Update(const std::string& value);
  [[nodiscard]] std::string Serialize(const std::string& key) override;

 private:
  std::variant<std::string, int64_t> value_;
};

}  // namespace libcache::db

#endif  // LIBCACHE_SRC_DB_STRING_OBJECT_HPP_
