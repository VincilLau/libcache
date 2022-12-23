#ifndef LIBCACHE_SRC_SNAPSHOT_RECORD_HPP_
#define LIBCACHE_SRC_SNAPSHOT_RECORD_HPP_

#include <fstream>
#include <memory>

#include "libcache/error.hpp"

namespace libcache::snapshot {

class RecordReader {
 public:
  static Status Open(const std::string& path,
                     std::unique_ptr<RecordReader>& reader);
  ~RecordReader() { ifs_.close(); }

  Status Read(std::string& record);

 private:
  RecordReader(std::ifstream ifs) : ifs_(std::move(ifs)) {}
  Status ReadChecksum(char buf[4]);
  Status ReadLen(uint64_t& len);
  Status ReadBody(uint64_t len, std::string& record);

  std::ifstream ifs_;
};

class RecordWriter {
 public:
  static Status Open(const std::string& path,
                     std::unique_ptr<RecordWriter>& writer);

  ~RecordWriter() { ofs_.close(); }

  Status Append(const std::string& record);

 private:
  RecordWriter(std::ofstream ofs) : ofs_(std::move(ofs)) {}

  std::ofstream ofs_;
};

}  // namespace libcache::snapshot

#endif  // LIBCACHE_SRC_SNAPSHOT_RECORD_HPP_
