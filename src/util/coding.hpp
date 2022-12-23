#ifndef LIBCACHE_SRC_UTIL_CODING_HPP_
#define LIBCACHE_SRC_UTIL_CODING_HPP_

#include <cstdint>
#include <string>

namespace libcache::util {

void AppendVarU64(std::string& buf, uint64_t u64);

void PutFixedU32(char buf[4], uint32_t u32);

}  // namespace libcache::util

#endif  // LIBCACHE_SRC_UTIL_CODING_HPP_
