#include "db/db.hpp"
#include "db/string_object.hpp"

using std::dynamic_pointer_cast;
using std::lock_guard;
using std::make_shared;
using std::mutex;
using std::optional;
using std::string;

namespace libcache::db {

optional<string> DB::Get(Status& status, const string& key) const {
  status = {};
  lock_guard<mutex> lock(mutex_);

  auto obj = GetObject(key);
  if (!obj) {
    return {};
  }

  if (!obj->IsString()) {
    status = Status{kWrongType};
    return {};
  }

  auto string_obj = dynamic_pointer_cast<StringObject>(obj);
  return string_obj->value();
}

optional<string> DB::Set(Status& status, const string& key, const string& value,
                         uint64_t flags, const Expiration& expiration) {
  status = {};

  flags &= NX | XX | KEEPTTL | GET;
  if (flags & (NX & XX)) {
    status = Status{kSyntaxError};
    return {};
  }
  if ((flags & KEEPTTL) && (expiration != NO_EXPIRE)) {
    status = Status{kSyntaxError};
    return {};
  }

  lock_guard<mutex> lock(mutex_);

  auto old_obj = GetObject(key);
  if (!old_obj) {
    if (flags & XX) {
      return {};
    }

    if (HasObject(key)) {
      DelObject(key);
    }

    auto new_obj = make_shared<StringObject>(value);
    PutObject(key, new_obj);
    if (expiration.px != INT64_MAX) {
      ExpireAfterMsec(key, expiration.px);
    } else if (expiration.pxat != INT64_MAX) {
      ExpireAtUnixMsec(key, expiration.pxat);
    }
    return (flags & GET) ? optional<string>{} : "OK";
  }

  if (flags & NX) {
    if (!(flags & GET)) {
      return {};
    }
    if (old_obj->IsString()) {
      return dynamic_pointer_cast<StringObject>(old_obj)->value();
    }
    status = Status{kWrongType};
    return {};
  }

  if (old_obj->IsString()) {
    auto string_obj = dynamic_pointer_cast<StringObject>(old_obj);
    auto old_string = string_obj->value();
    string_obj->set_value(value);

    if (expiration.px != INT64_MAX) {
      ExpireAfterMsec(key, expiration.px);
    } else if (expiration.pxat != INT64_MAX) {
      ExpireAtUnixMsec(key, expiration.pxat);
    } else if (!(flags & KEEPTTL) && string_obj->expire_at()) {
      RemoveExpire(string_obj);
    }

    return (flags & GET) ? std::move(old_string) : "OK";
  }

  if (flags & GET) {
    status = Status{kWrongType};
    return {};
  }

  DelObject(key);
  auto string_obj = make_shared<StringObject>(value);
  PutObject(key, string_obj);
  if (expiration.px != INT64_MAX) {
    ExpireAfterMsec(key, expiration.px);
  } else if (expiration.pxat != INT64_MAX) {
    ExpireAtUnixMsec(key, expiration.pxat);
  }
  return "OK";
}

}  // namespace libcache::db