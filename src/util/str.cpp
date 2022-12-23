#include "str.hpp"

#include <cstddef>

namespace libcache::util {

bool StrToU64(const char* str, uint64_t& u64) {
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

}  // namespace libcache::util
