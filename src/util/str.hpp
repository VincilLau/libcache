#ifndef LIBCACHE_SRC_UTIL_STR_HPP_
#define LIBCACHE_SRC_UTIL_STR_HPP_

#include <cstdint>

namespace libcache::util {

bool StrToU64(const char* str, uint64_t& u64);

}  // namespace libcache::util

#endif  // LIBCACHE_SRC_UTIL_STR_HPP_
