#include "object.hpp"

using libcache::expire::SteadyTimePoint;
using libcache::expire::SystemTimePoint;
using libcache::expire::TimePoint;
using std::string;

namespace libcache::db {

snapshot::Object Object::SnapshotObject(const string& key) const {
  snapshot::Object obj;
  obj.set_key(key);
  if (!expire_at_) {
    obj.set_pxat(INT64_MAX);
    return obj;
  }

  if (expire_at_->type() == TimePoint::Type::kSystem) {
    obj.set_pxat(expire_at_->Msec());
  } else {
    int64_t pxat =
        expire_at_->Msec() + SystemTimePoint::Now() - SteadyTimePoint::Now();
    obj.set_pxat(pxat);
  }
  return obj;
}

}  // namespace libcache::db
