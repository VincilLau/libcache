#include "checksum.hpp"

#include <crc32c/crc32c.h>

#include "coding.hpp"

using crc32c::Crc32c;
using std::string;

namespace libcache::util {

uint32_t Checksum(const string& record) {
  uint64_t len = record.size();
  string len_bytes;
  AppendVarU64(len_bytes, len);
  uint64_t crc = Crc32c(len_bytes);
  crc += Crc32c(record);
  while (crc > UINT32_MAX) {
    crc = (crc & UINT32_MAX) + (crc >> 32 & UINT32_MAX);
  }
  return crc;
}

}  // namespace libcache::util
