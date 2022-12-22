#ifndef LIBCACHE_SRC_STATUS_HPP_
#define LIBCACHE_SRC_STATUS_HPP_

#include "libcache/libcache.hpp"

namespace libcache {

inline void ThrowIfError(const Status& status) {
  if (status.Error()) {
    throw Exception(status);
  }
}

}  // namespace libcache

#endif  // LIBCACHE_SRC_STATUS_HPP_
