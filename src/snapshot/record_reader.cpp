#include "record_reader.hpp"

#include <cstring>
#include <filesystem>

#include "checksum.hpp"

using std::ifstream;
using std::string;
using std::unique_ptr;
using std::filesystem::exists;
using std::filesystem::is_regular_file;

namespace libcache::snapshot {

unique_ptr<RecordReader> RecordReader::Open(const string& path,
                                            Status& status) {
  status = {};

  if (!exists(path)) {
    status = Status{kNoSuchFile};
    return nullptr;
  }
  if (!is_regular_file(path)) {
    status = Status{kExpectARegularFile};
    return nullptr;
  }

  ifstream ifs(path);
  if (!ifs.is_open()) {
    status = Status{kIOError};
    return nullptr;
  }

  return unique_ptr<RecordReader>(new RecordReader(std::move(ifs)));
}

string RecordReader::Read(Status& status) {
  status = {};

  ifs_.peek();
  if (ifs_.eof()) {
    status = Status{kEof};
    return {};
  }

  char checksum_in_file[4] = {};
  status = ReadChecksum(checksum_in_file);
  if (status.Error()) {
    return {};
  }

  uint64_t len = 0;
  status = ReadLen(len);
  if (status.Error()) {
    return {};
  }

  if (!len) {
    status = Status{kCorrupt};
    return {};
  }

  string record;
  status = ReadBody(len, record);
  if (status.Error()) {
    return {};
  }

  uint32_t checksum = Checksum(record);
  char checksum_bytes[4] = {};
  PutFixedU32(checksum_bytes, checksum);
  if (memcmp(checksum_in_file, checksum_bytes, 4)) {
    status = Status{kCorrupt};
    return {};
  }

  return record;
}

Status RecordReader::Close() {
  assert(ifs_.is_open());
  ifs_.close();
  if (ifs_.fail()) {
    return Status{kIOError};
  }
  return {};
}

Status RecordReader::ReadChecksum(char buf[4]) {
  ifs_.read(buf, 4);
  if (ifs_.good()) {
    return {};
  }
  if (ifs_.eof()) {
    return Status{kCorrupt};
  }
  return Status{kIOError};
}

Status RecordReader::ReadLen(uint64_t& len) {
  constexpr size_t kMaxBytes = (64 + 6) / 7;
  len = 0;

  for (size_t i = 0; i < kMaxBytes; i++) {
    char ch = ifs_.get();
    uint8_t byte = *reinterpret_cast<uint8_t*>(&ch);
    if (byte > 0x7f) {
      len = (len << 7) + (byte & 0x7f);
      continue;
    }
    len = (len << 7) + byte;
    return {};
  }
  return Status{kCorrupt};
}

Status RecordReader::ReadBody(uint64_t len, string& record) {
  record.resize(len);
  ifs_.read(record.data(), len);
  if (ifs_.good()) {
    return {};
  }
  if (ifs_.eof()) {
    return Status{kCorrupt};
  }
  return Status{kIOError};
}

}  // namespace libcache::snapshot
