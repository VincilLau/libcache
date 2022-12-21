#ifndef LIBCACHE_SRC_SNAPSHOT_RECORD_WRITER_HPP_
#define LIBCACHE_SRC_SNAPSHOT_RECORD_WRITER_HPP_

#include <cassert>
#include <fstream>
#include <memory>
#include <string>

#include "libcache.hpp"

namespace libcache::snapshot {

class RecordWriter {
 public:
  [[nodiscard]] static std::unique_ptr<RecordWriter> Open(
      const std::string& path, Status& status);

  ~RecordWriter() { assert(!ofs_.is_open()); }

  [[nodiscard]] Status Append(const std::string& record);
  [[nodiscard]] Status Close();

 private:
  RecordWriter(std::ofstream ofs) : ofs_(std::move(ofs)) {}

  std::ofstream ofs_;
};

}  // namespace libcache::snapshot

#endif  // LIBCACHE_SRC_SNAPSHOT_RECORD_WRITER_HPP_
