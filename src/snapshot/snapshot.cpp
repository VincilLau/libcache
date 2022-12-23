#include "snapshot.hpp"

using std::string;
using std::unique_ptr;

namespace libcache::snapshot {

Status SnapshotWriter::Open(const string& path,
                            unique_ptr<SnapshotWriter>& writer) {
  unique_ptr<RecordWriter> record_writer;
  auto status = RecordWriter::Open(path, record_writer);
  if (status.error()) {
    return status;
  }
  writer = unique_ptr<SnapshotWriter>(new SnapshotWriter(move(record_writer)));
  return Status::OK();
}

Status SnapshotReader::Open(const string& path,
                            unique_ptr<SnapshotReader>& reader) {
  unique_ptr<RecordReader> record_reader;
  auto status = RecordReader::Open(path, record_reader);
  if (status.error()) {
    return status;
  }
  reader = unique_ptr<SnapshotReader>(new SnapshotReader(move(record_reader)));
  return Status::OK();
}

Status SnapshotReader::Read(snapshot::Object& obj) {
  string record;
  auto status = record_reader_->Read(record);
  if (status.error()) {
    return status;
  }

  auto ok = obj.ParseFromString(record);
  if (!ok) {
    return Status::Corrupt();
  }
  return Status::OK();
}

}  // namespace libcache::snapshot
