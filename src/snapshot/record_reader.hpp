#ifndef LIBCACHE_SRC_SNAPSHOT_RECORD_READER_HPP_
#define LIBCACHE_SRC_SNAPSHOT_RECORD_READER_HPP_

#include <cassert>
#include <fstream>
#include <memory>
#include <string>

#include "libcache/libcache.hpp"

namespace libcache::snapshot {

class RecordReader {
 public:
  [[nodiscard]] static std::unique_ptr<RecordReader> Open(
      const std::string& path, Status& status);

  ~RecordReader() { assert(!ifs_.is_open()); }

  [[nodiscard]] std::string Read(Status& status);
  [[nodiscard]] Status Close();

 private:
  RecordReader(std::ifstream ifs) : ifs_(std::move(ifs)) {}
  [[nodiscard]] Status ReadChecksum(char buf[4]);
  [[nodiscard]] Status ReadLen(uint64_t& len);
  [[nodiscard]] Status ReadBody(uint64_t len, std::string& record);

  std::ifstream ifs_;
};

}  // namespace libcache::snapshot

#endif  // LIBCACHE_SRC_SNAPSHOT_RECORD_READER_HPP_
