#ifndef LIBCACHE_SRC_DB_OBJECT_HPP_
#define LIBCACHE_SRC_DB_OBJECT_HPP_

#include <cassert>
#include <memory>

#include "expire/time_point.hpp"
#include "libcache/libcache.hpp"
#include "snapshot.pb.h"

namespace libcache::db {

class Object {
 public:
  struct ExpireHelper {
    std::function<expire::BootTime(const std::string&, int64_t)> px;
    std::function<expire::UnixTime(const std::string&, int64_t)> pxat;
    std::function<void(const expire::TimePoint*)> persist;
  };

  explicit Object(std::string key, ExpireHelper expire_helper)
      : key_(std::move(key)), expire_helper_(std::move(expire_helper)) {}
  virtual ~Object() {
    if (HasExpire()) {
      Persist();
    }
  }
  const std::string& key() const { return key_; }

  virtual Type type() const = 0;
  bool IsString() const { return type() == Type::kString; }

  virtual Encoding encoding() const = 0;

  int64_t idletime() const { return expire::UnixTime::Now() - access_; }
  void Touch() { access_ = expire::UnixTime::Now(); }

  int64_t pttl() const {
    assert(HasExpire());
    return expire_->pttl();
  }
  int64_t expire_unix() const {
    assert(HasExpire());
    return expire_->ToUnixTime();
  }

  bool HasExpire() const { return expire_.get(); }
  void Px(int64_t ms);
  void Pxat(int64_t ms);
  void Persist();

  virtual std::string Serialize() = 0;
  void Parse(const snapshot::Object& obj);

 protected:
  snapshot::Object SnapshotObject() const;

 private:
  ExpireHelper expire_helper_;
  std::string key_;
  int64_t access_ = expire::UnixTime::Now();
  std::shared_ptr<expire::TimePoint> expire_;
};

}  // namespace libcache::db

#endif  // LIBCACHE_SRC_DB_OBJECT_HPP_
