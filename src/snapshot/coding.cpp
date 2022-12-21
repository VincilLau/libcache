#include "coding.hpp"

using std::istream;
using std::string;

namespace libcache::snapshot {

void AppendVarU64(string& buf, uint64_t u64) {
  while (u64 > 0x7f) {
    uint8_t byte = u64 & 0x7f | 0x80;
    u64 >>= 7;
    char* ch = reinterpret_cast<char*>(&byte);
    buf.push_back(*ch);
  }
  uint8_t byte = u64;
  char* ch = reinterpret_cast<char*>(&byte);
  buf.push_back(*ch);
}

void PutFixedU32(char buf[4], uint32_t u32) {
  uint8_t* bytes = reinterpret_cast<uint8_t*>(buf);
  bytes[0] = u32 & 0x7f;
  bytes[1] = u32 >> 8 & 0x7f;
  bytes[2] = u32 >> 16 & 0x7f;
  bytes[3] = u32 >> 24 & 0x7f;
}

}  // namespace libcache::snapshot
