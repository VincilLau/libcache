#ifndef LIBCACHE_INCLUDE_LIBCACHE_ERROR_HPP_
#define LIBCACHE_INCLUDE_LIBCACHE_ERROR_HPP_

#include <cstddef>
#include <stdexcept>
#include <string>

namespace libcache {

static constexpr size_t kOK = 0;
static constexpr size_t kDBIndexOutOfRange = 1;
static constexpr size_t kWrongType = 2;
static constexpr size_t kSyntaxError = 3;
static constexpr size_t kInvalidFlags = 4;
static constexpr size_t kNoSuchFile = 5;
static constexpr size_t kNotAFile = 6;
static constexpr size_t kIOError = 7;
static constexpr size_t kEof = 8;
static constexpr size_t kCorrupt = 9;
static constexpr size_t kChecksumNotMatch = 10;
static constexpr size_t kInvalidOptions = 11;
static constexpr size_t kInvalidInt64 = 12;

class Status {
 public:
  Status(size_t code, std::string what) : code_(code), what_(std::move(what)) {}

  size_t code() const { return code_; }
  const std::string& what() const { return what_; }
  bool ok() const { return code_ == kOK; }
  bool error() const { return code_ != kOK; }

  void ThrowIfError() const;

  static Status OK() { return {kOK, "OK"}; }
  static Status DBIndexOutOfRange() {
    return {kDBIndexOutOfRange, "DB index out of range"};
  }
  static Status WrongType() {
    return {
        kWrongType,
        "WRONGTYPE Operation against a key holding the wrong kind of value"};
  }
  static Status SyntaxError() { return {kSyntaxError, "syntax error"}; }
  static Status InvalidFlags(std::string what) {
    return {kInvalidFlags, std::move(what)};
  }
  static Status NoSuchFile(const std::string& path) {
    return {kNoSuchFile, path + " doesn't exist"};
  }
  static Status NotAFile(const std::string& path) {
    return {kNotAFile, path + " isn't a regular file"};
  }
  static Status IOError() { return {kIOError, "IO error"}; }
  static Status Eof() { return {kEof, "end of file"}; }
  static Status Corrupt() { return {kCorrupt, "snapshot file is corrupt"}; }
  static Status ChecksumNotMatch() {
    return {kChecksumNotMatch, "checksum doesn't match"};
  }
  static Status InvalidOptions(std::string what) {
    return {kInvalidOptions, std::move(what)};
  }
  static Status InvalidInt64() {
    return {kInvalidInt64, "value is not an integer or out of range"};
  }

 private:
  size_t code_;
  std::string what_;
};

class Exception : public std::exception {
 public:
  explicit Exception(Status status) : status_(std::move(status)) {}

  const Status& status() { return status_; }
  size_t code() const { return status_.code(); }
  const char* what() const noexcept override { return status_.what().c_str(); }

 private:
  Status status_;
};

inline void Status::ThrowIfError() const {
  if (error()) {
    throw Exception(std::move(*this));
  }
}

}  // namespace libcache

#endif  // LIBCACHE_INCLUDE_LIBCACHE_ERROR_HPP_
