#include "snapshot.hpp"

#include "db/string_object.hpp"

using std::make_shared;
using std::move;
using std::shared_ptr;
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

Status SnapshotReader::Read(shared_ptr<db::Object>& obj,
                            db::Object::ExpireHelper helper) {
  string record;
  auto status = record_reader_->Read(record);
  if (status.error()) {
    return status;
  }

  snapshot::Object snapshot_obj;
  auto ok = snapshot_obj.ParseFromString(record);
  if (!ok) {
    return Status::Corrupt();
  }

  if (snapshot_obj.has_string_object()) {
    auto obj = make_shared<db::StringObject>(
        snapshot_obj.key(), move(helper), snapshot_obj.string_object().value());
    obj->Parse(snapshot_obj);
    return Status::OK();
  }

  return Status::OK();
}

}  // namespace libcache::snapshot
