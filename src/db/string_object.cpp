#include "string_object.hpp"

#include "snapshot.pb.h"
#include "util/str.hpp"

using libcache::util::StrToU64;
using std::string;

namespace libcache::db {

Encoding StringObject::encoding() const {
  if (IsRaw()) {
    return Encoding::kRaw;
  }
  return Encoding::kInt;
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

string StringObject::Serialize() {
  auto obj = SnapshotObject();
  obj.mutable_string_object()->set_value(string());
  return obj.SerializeAsString();
}

}  // namespace libcache::db
