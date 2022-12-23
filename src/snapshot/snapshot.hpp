#ifndef LIBCACHE_SRC_SNAPSHOT_SNAPSHOT_HPP_
#define LIBCACHE_SRC_SNAPSHOT_SNAPSHOT_HPP_

#include <memory>

#include "db/object.hpp"
#include "libcache/error.hpp"
#include "record.hpp"
#include "snapshot.pb.h"

namespace libcache::snapshot {

class SnapshotReader {
 public:
  static Status Open(const std::string& path,
                     std::unique_ptr<SnapshotReader>& reader);

  Status Read(std::shared_ptr<db::Object>& obj,
              db::Object::ExpireHelper helper);

 private:
  SnapshotReader(std::unique_ptr<RecordReader> record_reader)
      : record_reader_(std::move(record_reader)) {}

  std::unique_ptr<RecordReader> record_reader_;
};

class SnapshotWriter {
 public:
  static Status Open(const std::string& path,
                     std::unique_ptr<SnapshotWriter>& writer);

  Status Append(const std::string& key, std::shared_ptr<db::Object> obj) {
    return record_writer_->Append(obj->Serialize());
  }

 private:
  explicit SnapshotWriter(std::unique_ptr<RecordWriter> record_writer)
      : record_writer_(std::move(record_writer)) {}

  std::unique_ptr<RecordWriter> record_writer_;
};

}  // namespace libcache::snapshot

#endif  // LIBCACHE_SRC_SNAPSHOT_SNAPSHOT_HPP_
