#ifndef LIBCACHE_INCLUDE_LIBCACHE_TYPES_HPP_
#define LIBCACHE_INCLUDE_LIBCACHE_TYPES_HPP_

#include <cstdint>
#include <string>

namespace libcache {

enum class Type {
  kNone,
  kString,
};

enum class Encoding {
  kRaw,
  kInt,
};

}  // namespace libcache

#endif  // LIBCACHE_INCLUDE_LIBCACHE_TYPES_HPP_
