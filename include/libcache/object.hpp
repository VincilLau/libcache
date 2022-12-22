#ifndef LIBCACHE_INCLUDE_LIBCACHE_OBJECT_HPP_
#define LIBCACHE_INCLUDE_LIBCACHE_OBJECT_HPP_

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

#endif  // LIBCACHE_INCLUDE_LIBCACHE_OBJECT_HPP_
