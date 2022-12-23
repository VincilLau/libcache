#include "record.hpp"

#include <cstring>
#include <filesystem>

#include "util/checksum.hpp"
#include "util/coding.hpp"

using libcache::util::AppendVarU64;
using libcache::util::Checksum;
using libcache::util::PutFixedU32;
using std::flush;
using std::ifstream;
using std::ios;
using std::ofstream;
using std::string;
using std::unique_ptr;
using std::filesystem::exists;
using std::filesystem::is_regular_file;

namespace libcache::snapshot {

Status RecordReader::Open(const string& path,
                          unique_ptr<RecordReader>& reader) {
  if (!exists(path)) {
    return Status::NoSuchFile(path);
  }
  if (!is_regular_file(path)) {
    return Status::NotAFile(path);
  }

  ifstream ifs(path);
  if (!ifs.is_open()) {
    return Status::IOError();
  }

  reader = unique_ptr<RecordReader>(new RecordReader(move(ifs)));
  return Status::OK();
}

Status RecordReader::Read(string& record) {
  ifs_.peek();
  if (ifs_.eof()) {
    return Status::Eof();
  }

  char checksum_in_file[4] = {};
  auto status = ReadChecksum(checksum_in_file);
  if (status.error()) {
    return status;
  }

  uint64_t len = 0;
  status = ReadLen(len);
  if (status.error()) {
    return status;
  }

  if (!len) {
    return Status::Corrupt();
  }

  status = ReadBody(len, record);
  if (status.error()) {
    return status;
  }

  uint32_t checksum = Checksum(record);
  char checksum_bytes[4] = {};
  PutFixedU32(checksum_bytes, checksum);
  if (memcmp(checksum_in_file, checksum_bytes, 4)) {
    return Status::ChecksumNotMatch();
  }

  return Status::OK();
}

Status RecordReader::ReadChecksum(char buf[4]) {
  ifs_.read(buf, 4);
  if (ifs_.good()) {
    return Status::OK();
  }
  if (ifs_.eof()) {
    return Status::Corrupt();
  }
  return Status::IOError();
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
    return Status::OK();
  }
  return Status::Corrupt();
}

Status RecordReader::ReadBody(uint64_t len, string& record) {
  record.resize(len);
  ifs_.read(record.data(), len);
  if (ifs_.good()) {
    return Status::OK();
  }
  if (ifs_.eof()) {
    return Status::Corrupt();
  }
  return Status::IOError();
}

Status RecordWriter::Open(const string& path,
                          unique_ptr<RecordWriter>& writer) {
  ofstream ofs(path, ios::out | ios::trunc);
  if (!ofs.is_open()) {
    return Status::IOError();
  }
  writer = unique_ptr<RecordWriter>(new RecordWriter{move(ofs)});
  return Status::OK();
}

Status RecordWriter::Append(const string& record) {
  string header(4, '\0');
  AppendVarU64(header, record.size());
  uint32_t checksum = Checksum(record);
  PutFixedU32(header.data(), checksum);
  ofs_ << header;
  if (ofs_.fail()) {
    return Status::IOError();
  }
  ofs_ << record;
  if (ofs_.fail()) {
    return Status::IOError();
  }
  ofs_ << flush;
  if (ofs_.fail()) {
    return Status::IOError();
  }
  return Status::OK();
}

}  // namespace libcache::snapshot
