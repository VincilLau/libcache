#ifndef LIBCACHE_INCLUDE_LIBCACHE_OPTIONS_HPP_
#define LIBCACHE_INCLUDE_LIBCACHE_OPTIONS_HPP_

#include <cstddef>
#include <vector>

namespace libcache {

struct DBOptions {
  size_t time_wheel_size = 1;
};

struct Options {
  size_t timer_interval = 1000;
  std::vector<DBOptions> db_options_array = {DBOptions{}};
};

}  // namespace libcache

#endif  // LIBCACHE_INCLUDE_LIBCACHE_OPTIONS_HPP_
