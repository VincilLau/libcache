#ifndef LIBCACHE_SRC_UTIL_CHECKSUM_HPP_
#define LIBCACHE_SRC_UTIL_CHECKSUM_HPP_

#include <cstdint>
#include <string>

namespace libcache::util {

uint32_t Checksum(const std::string& record);

}  // namespace libcache::util

#endif  // LIBCACHE_SRC_UTIL_CHECKSUM_HPP_
