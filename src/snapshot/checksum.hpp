#ifndef LIBCACHE_SRC_SNAPSHOT_CHECKSUM_HPP_
#define LIBCACHE_SRC_SNAPSHOT_CHECKSUM_HPP_

#include <crc32c/crc32c.h>

#include <cstdint>
#include <string>

#include "coding.hpp"

namespace libcache::snapshot {

[[nodiscard]] inline static uint32_t Checksum(const std::string& record) {
  uint64_t len = record.size();
  std::string len_bytes;
  AppendVarU64(len_bytes, len);
  uint64_t crc = crc32c::Crc32c(len_bytes);
  crc += crc32c::Crc32c(record);
  while (crc > UINT32_MAX) {
    crc = (crc & UINT32_MAX) + (crc >> 32 & UINT32_MAX);
  }
  return crc;
}

}  // namespace libcache::snapshot

#endif  // LIBCACHE_SRC_SNAPSHOT_CHECKSUM_HPP_
