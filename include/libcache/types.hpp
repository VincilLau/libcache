#ifndef LIBCACHE_INCLUDE_LIBCACHE_TYPES_HPP_
#define LIBCACHE_INCLUDE_LIBCACHE_TYPES_HPP_

#include <cstdint>
#include <string>

namespace libcache {

using Key = std::string;
using Integer = int64_t;
using String = std::string;

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
