#include <cassert>

#include "db/db.hpp"

using libcache::expire::SteadyTimePoint;
using libcache::expire::SystemTimePoint;
using libcache::expire::TimePoint;
using std::lock_guard;
using std::mutex;
using std::string;
using std::chrono::duration_cast;

namespace libcache::db {

int64_t DB::Persist(Status& status, const string& key) {
  status = {};

  lock_guard<mutex> lock(mutex_);
  auto obj = GetObject(key);
  if (!obj) {
    return 0;
  }
  auto expire_at = obj->expire_at();
  if (!expire_at) {
    return 0;
  }
  RemoveExpire(obj);
  return 1;
}

int64_t DB::PExpire(Status& status, const string& key, int64_t milliseconds,
                    uint64_t flags) {
  status = {};

  flags &= NX | XX | GT | LT;
  if (flags & NX && flags != NX) {
    status = Status{kNxAndXxGtOrLtNotCompatible};
    return INT64_MIN;
  }
  if (flags & GT && flags & LT) {
    status = Status{kGtAndLtNotCompatible};
    return INT64_MIN;
  }

  lock_guard<mutex> lock(mutex_);
  auto obj = GetObject(key);
  if (!obj) {
    return 0;
  }

  auto expire_at = obj->expire_at();
  if (expire_at) {
    if (flags & NX) {
      return 0;
    }
    if (flags & GT) {
      if (expire_at->Pttl() <= milliseconds) {
        return 0;
      }
    }
    if (flags & LT) {
      if (expire_at->Pttl() >= milliseconds) {
        return 0;
      }
    }
    ExpireAfterMsec(key, milliseconds);
    return 1;
  }

  if (flags & XX) {
    return 0;
  }
  ExpireAfterMsec(key, milliseconds);
  return 1;
}

int64_t DB::PExpireAt(Status& status, const string& key,
                      int64_t unix_time_milliseconds, uint64_t flags) {
  status = {};

  flags &= NX | XX | GT | LT;
  if (flags & NX && flags != NX) {
    status = Status{kNxAndXxGtOrLtNotCompatible};
    return INT64_MIN;
  }
  if (flags & GT && flags & LT) {
    status = Status{kGtAndLtNotCompatible};
    return INT64_MIN;
  }

  lock_guard<mutex> lock(mutex_);
  auto obj = GetObject(key);
  if (!obj) {
    return 0;
  }

  auto expire_at = obj->expire_at();
  if (expire_at) {
    if (flags & NX) {
      return 0;
    }
    if (flags & GT) {
      if (expire_at->Msec() <= unix_time_milliseconds) {
        return 0;
      }
    }
    if (flags & LT) {
      if (expire_at->Msec() >= unix_time_milliseconds) {
        return 0;
      }
    }
    ExpireAtUnixMsec(key, unix_time_milliseconds);
    return 1;
  }

  if (flags & XX) {
    return 0;
  }
  ExpireAtUnixMsec(key, unix_time_milliseconds);
  return 1;
}

int64_t DB::PExpireTime(Status& status, const string& key) const {
  status = {};
  lock_guard<mutex> lock(mutex_);

  auto obj = GetObject(key);
  if (!obj) {
    return -2;
  }
  auto expire_at = obj->expire_at();
  if (!expire_at) {
    return -1;
  }

  if (expire_at->type() == TimePoint::Type::kSystem) {
    return expire_at->Msec();
  }

  // 相对过期时间的时间戳只能估算。
  int64_t system_now_msec =
      duration_cast<TimePoint::Duration>(
          SystemTimePoint::Clock::now().time_since_epoch())
          .count();
  int64_t steady_now_msec =
      duration_cast<TimePoint::Duration>(
          SteadyTimePoint::Clock::now().time_since_epoch())
          .count();
  return expire_at->Msec() + (system_now_msec - steady_now_msec);
}

int64_t DB::Pttl(Status& status, const string& key) const {
  status = {};
  lock_guard<mutex> lock(mutex_);

  auto obj = GetObject(key);
  if (!obj) {
    return -2;
  }
  auto expire_at = obj->expire_at();
  if (!expire_at) {
    return -1;
  }
  return expire_at->Pttl();
}

}  // namespace libcache::db
