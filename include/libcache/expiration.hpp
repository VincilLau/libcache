#ifndef LIBCACHE_INCLUDE_LIBCACHE_EXPIRATION_HPP_
#define LIBCACHE_INCLUDE_LIBCACHE_EXPIRATION_HPP_

#include <cstdint>

namespace libcache {

struct Expiration {
  int64_t px;
  int64_t pxat;

  bool operator==(const Expiration& other) const {
    return px == other.px && pxat == other.pxat;
  }

  bool operator!=(const Expiration& other) const {
    return px != other.px || pxat != other.pxat;
  }
};

#define NO_EXPIRE (libcache::Expiration{.px = INT64_MAX, .pxat = INT64_MAX})
#define EX(sec) (libcache::Expiration{.px = (sec)*1000, .pxat = INT64_MAX})
#define EXAT(ms) (libcache::Expiration{.px = INT64_MAX, .pxat = (ms)*1000})
#define PX(sec) (libcache::Expiration{.px = (sec), .pxat = INT64_MAX})
#define PXAT(ms) (libcache::Expiration{.px = INT64_MAX, .pxat = (ms)})

}  // namespace libcache

#endif  // LIBCACHE_INCLUDE_LIBCACHE_EXPIRATION_HPP_
