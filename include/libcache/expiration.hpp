#ifndef LIBCACHE_INCLUDE_LIBCACHE_EXPIRATION_HPP_
#define LIBCACHE_INCLUDE_LIBCACHE_EXPIRATION_HPP_

#include <cstdint>

namespace libcache {

// 过期时间，单位毫秒。
struct Expiration {
  int64_t px;
  int64_t pxat;

  [[nodiscard]] bool operator==(const Expiration& other) const {
    return px == other.px && pxat == other.pxat;
  }

  [[nodiscard]] bool operator!=(const Expiration& other) const {
    return px != other.px || pxat != other.pxat;
  }
};

#define NO_EXPIRE (libcache::Expiration{.px = INT64_MAX, .pxat = INT64_MAX})
#define EX(sec) (libcache::Expiration{.px = (sec)*1000, .pxat = INT64_MAX})
#define EXAT(msec) (libcache::Expiration{.px = INT64_MAX, .pxat = (msec)*1000})
#define PX(sec) (libcache::Expiration{.px = (sec), .pxat = INT64_MAX})
#define PXAT(msec) (libcache::Expiration{.px = INT64_MAX, .pxat = (msec)})

}  // namespace libcache

#endif  // LIBCACHE_INCLUDE_LIBCACHE_EXPIRATION_HPP_
