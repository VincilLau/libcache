#include "record_writer.hpp"

#include "checksum.hpp"
#include "coding.hpp"

using std::flush;
using std::ios;
using std::ofstream;
using std::string;
using std::unique_ptr;

namespace libcache::snapshot {

unique_ptr<RecordWriter> RecordWriter::Open(const string& path,
                                            Status& status) {
  status = {};
  ofstream ofs(path, ios::out | ios::trunc);
  if (!ofs.is_open()) {
    status = Status{kIOError};
    return nullptr;
  }
  return unique_ptr<RecordWriter>(new RecordWriter{std::move(ofs)});
}

Status RecordWriter::Append(const string& record) {
  string header(4, '\0');
  AppendVarU64(header, record.size());
  uint32_t checksum = Checksum(record);
  PutFixedU32(header.data(), checksum);
  ofs_ << header;
  if (ofs_.fail()) {
    return Status{kIOError};
  }
  ofs_ << record;
  if (ofs_.fail()) {
    return Status{kIOError};
  }
  ofs_ << flush;
  if (ofs_.fail()) {
    return Status{kIOError};
  }
  return {};
}

Status RecordWriter::Close() {
  assert(ofs_.is_open());
  ofs_.close();
  if (ofs_.fail()) {
    return Status{kIOError};
  }
  return {};
}

}  // namespace libcache::snapshot
