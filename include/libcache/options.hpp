#ifndef LIBCACHE_INCLUDE_LIBCACHE_OPTIONS_HPP_
#define LIBCACHE_INCLUDE_LIBCACHE_OPTIONS_HPP_

#include <cstddef>

namespace libcache {

struct Options {
  size_t db_count = 1;
  size_t expire_timer_interval = 1;
  size_t system_time_wheel_bucket_count = 1;
  size_t steady_time_wheel_bucket_count = 1;
};

}  // namespace libcache

#endif  // LIBCACHE_INCLUDE_LIBCACHE_OPTIONS_HPP_
