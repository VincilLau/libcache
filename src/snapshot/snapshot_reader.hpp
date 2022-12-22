#ifndef LIBCACHE_SRC_SNAPSHOT_SNAPSHOT_READER_HPP_
#define LIBCACHE_SRC_SNAPSHOT_SNAPSHOT_READER_HPP_

#include <memory>
#include <string>

#include "libcache/libcache.hpp"
#include "record_reader.hpp"
#include "snapshot.pb.h"

namespace libcache::snapshot {

class SnapshotReader {
 public:
  [[nodiscard]] static std::unique_ptr<SnapshotReader> Open(
      const std::string& path, Status& status) {
    auto record_reader = RecordReader::Open(path, status);
    if (status.Error()) {
      return nullptr;
    }
    return std::unique_ptr<SnapshotReader>(
        new SnapshotReader(std::move(record_reader)));
  }

  [[nodiscard]] snapshot::Object Read(Status& status) {
    std::string record = record_reader_->Read(status);
    if (status.Error()) {
      return {};
    }
    snapshot::Object obj;
    auto ok = obj.ParseFromString(record);
    if (!ok) {
      status = Status{kCorrupt};
      return {};
    }
    return obj;
  }

  [[nodiscard]] Status Close() { return record_reader_->Close(); }

 private:
  SnapshotReader(std::unique_ptr<RecordReader> record_reader)
      : record_reader_(std::move(record_reader)) {}

  std::unique_ptr<RecordReader> record_reader_;
};

}  // namespace libcache::snapshot

#endif  // LIBCACHE_SRC_SNAPSHOT_SNAPSHOT_READER_HPP_
