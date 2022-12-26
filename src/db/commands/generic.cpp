#include <cassert>

#include "db/db.hpp"

using std::lock_guard;
using std::mutex;
using std::optional;
using std::string;
using std::vector;

namespace libcache::db {

optional<Encoding> DB::ObjectEncoding(const string& key) const {
  lock_guard<mutex> lock(mutex_);

  auto obj = GetObject(key);
  if (!obj) {
    return {};
  }
  return obj->encoding();
}

optional<int64_t> DB::ObjectIdletime(const string& key) const {
  lock_guard<mutex> lock(mutex_);

  auto obj = GetObject(key);
  if (!obj) {
    return {};
  }
  return obj->idletime() / 1000;
}

int64_t DB::Persist(const string& key) const {
  lock_guard<mutex> lock(mutex_);

  auto obj = GetObject(key);
  if (!obj) {
    return 0;
  }
  obj->Touch();

  if (obj->HasExpire()) {
    obj->Persist();
  }
  return 1;
}

int64_t DB::PExpire(Status& status, const string& key, int64_t milliseconds,
                    uint64_t flags) {
  status = Status::OK();

  flags &= NX | XX | GT | LT;
  if (flags & NX && flags != NX) {
    status = Status::InvalidFlags(
        "NX and XX, GT or LT flags at the same time are not compatible");
    return INT64_MIN;
  }
  if (flags & GT && flags & LT) {
    status = Status::InvalidFlags(
        "GT and LT flags at the same time are not compatible");
    return INT64_MIN;
  }

  lock_guard<mutex> lock(mutex_);
  auto obj = GetObject(key);
  if (!obj) {
    return 0;
  }
  obj->Touch();

  int64_t pttl = obj->pttl();
  if (obj->HasExpire()) {
    if (flags & NX) {
      return 0;
    }
    if (flags & GT) {
      if (pttl <= milliseconds) {
        return 0;
      }
    }
    if (flags & LT) {
      if (pttl >= milliseconds) {
        return 0;
      }
    }
    obj->Px(milliseconds);
    return 1;
  }

  if (flags & XX) {
    return 0;
  }
  obj->Px(milliseconds);
  return 1;
}

int64_t DB::PExpireAt(Status& status, const string& key,
                      int64_t unix_time_milliseconds, uint64_t flags) {
  status = Status::OK();

  flags &= NX | XX | GT | LT;
  if (flags & NX && flags != NX) {
    status = Status::InvalidFlags(
        "NX and XX, GT or LT flags at the same time are not compatible");
    return INT64_MIN;
  }
  if (flags & GT && flags & LT) {
    status = Status::InvalidFlags(
        "GT and LT flags at the same time are not compatible");
    return INT64_MIN;
  }

  lock_guard<mutex> lock(mutex_);
  auto obj = GetObject(key);
  if (!obj) {
    return 0;
  }
  obj->Touch();

  if (obj->HasExpire()) {
    if (flags & NX) {
      return 0;
    }
    if (flags & GT) {
      if (obj->expire_unix() <= unix_time_milliseconds) {
        return 0;
      }
    }
    if (flags & LT) {
      if (obj->expire_unix() >= unix_time_milliseconds) {
        return 0;
      }
    }
    obj->Pxat(unix_time_milliseconds);
    return 1;
  }

  if (flags & XX) {
    return 0;
  }
  obj->Pxat(unix_time_milliseconds);
  return 1;
}

int64_t DB::PExpireTime(const string& key) const {
  lock_guard<mutex> lock(mutex_);

  auto obj = GetObject(key);
  if (!obj) {
    return -2;
  }

  if (obj->HasExpire()) {
    return -1;
  }
  return obj->expire_unix();
}

int64_t DB::Pttl(const string& key) const {
  lock_guard<mutex> lock(mutex_);

  auto obj = GetObject(key);
  if (!obj) {
    return -2;
  }

  if (!obj->HasExpire()) {
    return -1;
  }
  return obj->pttl();
}

int64_t DB::Touch(const vector<string>& keys) {
  lock_guard<mutex> lock(mutex_);

  int64_t count = 0;
  for (const auto& key : keys) {
    auto obj = GetObject(key);
    if (obj) {
      obj->Touch();
      count++;
    }
  }
  return count;
}

enum Type DB::Type(const string& key) const {
  lock_guard<mutex> lock(mutex_);

  auto obj = GetObject(key);
  if (!obj) {
    return Type::kNone;
  }
  return obj->type();
}

}  // namespace libcache::db
