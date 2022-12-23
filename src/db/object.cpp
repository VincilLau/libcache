#include "object.hpp"

using libcache::expire::BootTime;
using libcache::expire::TimePoint;
using libcache::expire::UnixTime;
using std::dynamic_pointer_cast;
using std::make_shared;

namespace libcache::db {

void Object::Px(int64_t ms) {
  assert(!HasExpire());
  auto tp = expire_helper_.px(key_, ms);
  auto expire = make_shared<BootTime>(tp);
  expire_ = dynamic_pointer_cast<TimePoint>(expire);
}

void Object::Pxat(int64_t ms) {
  assert(!HasExpire());
  auto tp = expire_helper_.pxat(key_, ms);
  auto expire = make_shared<UnixTime>(tp);
  expire_ = dynamic_pointer_cast<TimePoint>(expire);
}

void Object::Persist() {
  assert(HasExpire());
  expire_helper_.persist(expire_.get());
  expire_.reset();
}

snapshot::Object Object::SnapshotObject() const {
  snapshot::Object obj;
  obj.set_key(key_);
  if (!HasExpire()) {
    obj.set_pxat(INT64_MAX);
    return obj;
  }
  obj.set_pxat(expire_->ToUnixTime());
  return obj;
}

void Object::Parse(const snapshot::Object& obj) {
  key_ = obj.key();
  if (obj.pxat() != INT64_MAX) {
    Pxat(obj.pxat());
  }
}

}  // namespace libcache::db
