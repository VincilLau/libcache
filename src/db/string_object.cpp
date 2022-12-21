#include "string_object.hpp"

#include <iostream>

#include "snapshot.pb.h"

using std::get_if;
using std::string;

namespace libcache::db {

Encoding StringObject::encoding() const {
  auto str = get_if<string>(&value_);
  if (str) {
    return Encoding::kRaw;
  }
  assert(get_if<int64_t>(&value_));
  return Encoding::kInt;
}

[[nodiscard]] static bool StrToU64(const char* str, uint64_t& u64) {
  if (*str == '\0') {
    return false;
  }

  u64 = 0;

  for (size_t i = 0; str[i]; i++) {
    char ch = str[i];
    if (ch < '0' || ch > '9') {
      return false;
    }
    if (u64 > UINT64_MAX / 10) {
      return false;
    }
    u64 *= 10;
    uint64_t digit = ch - '0';
    if (u64 > UINT64_MAX - digit) {
      return false;
    }
    u64 += digit;
  }

  return true;
}

void StringObject::Update(const string& value) {
  if (value.empty()) {
    value_ = value;
    return;
  }

  if (value[0] != '-') {
    uint64_t u64 = 0;
    bool is_int = StrToU64(value.c_str(), u64);
    if (!is_int || u64 > INT64_MAX) {
      value_ = value;
    } else {
      value_ = static_cast<int64_t>(u64);
    }
    return;
  }

  uint64_t u64 = 0;
  bool is_int = StrToU64(value.c_str() + 1, u64);
  if (!is_int || u64 > (1ULL << 63)) {
    value_ = value;
  } else {
    value_ = -static_cast<int64_t>(u64);
  }
}

string StringObject::Serialize(const string& key) {
  auto obj = SnapshotObject(key);
  obj.mutable_string_object()->set_value(String());
  return obj.SerializeAsString();
}

}  // namespace libcache::db
