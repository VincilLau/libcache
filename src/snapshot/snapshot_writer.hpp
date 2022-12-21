#ifndef LIBCACHE_SRC_SNAPSHOT_SNAPSHOT_WRITER_HPP_
#define LIBCACHE_SRC_SNAPSHOT_SNAPSHOT_WRITER_HPP_

#include <memory>
#include <string>

#include "db/object.hpp"
#include "libcache.hpp"
#include "record_writer.hpp"

namespace libcache::snapshot {

class SnapshotWriter {
 public:
  [[nodiscard]] static std::unique_ptr<SnapshotWriter> Open(
      const std::string& path, Status& status) {
    auto record_writer = RecordWriter::Open(path, status);
    if (status.Error()) {
      return nullptr;
    }
    return std::unique_ptr<SnapshotWriter>(
        new SnapshotWriter(std::move(record_writer)));
  }

  [[nodiscard]] Status Append(const std::string& key,
                              std::shared_ptr<db::Object> obj) {
    return record_writer_->Append(obj->Serialize(key));
  }
  [[nodiscard]] Status Close() { return record_writer_->Close(); }

 private:
  explicit SnapshotWriter(std::unique_ptr<RecordWriter> record_writer)
      : record_writer_(std::move(record_writer)) {}

  std::unique_ptr<RecordWriter> record_writer_;
};

}  // namespace libcache::snapshot

#endif  // LIBCACHE_SRC_SNAPSHOT_SNAPSHOT_WRITER_HPP_
