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
  status = Status::OK();
  lock_guard<mutex> lock(mutex_);

  auto obj = GetObject(key);
  if (!obj) {
    return {};
  }
  obj->Touch();

  if (!obj->IsString()) {
    status = Status::WrongType();
    return {};
  }

  auto string_obj = dynamic_pointer_cast<StringObject>(obj);
  return string_obj->String();
}

optional<string> DB::Set(Status& status, const string& key, const string& value,
                         uint64_t flags, const Expiration& expiration) {
  status = Status::OK();

  flags &= NX | XX | KEEPTTL | GET;
  if (flags & (NX & XX)) {
    status = Status::SyntaxError();
    return {};
  }
  if ((flags & KEEPTTL) && (expiration != NO_EXPIRE)) {
    status = Status::SyntaxError();
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

    auto new_obj = make_shared<StringObject>(key, expire_helper(), value);
    PutObject(key, new_obj);
    if (expiration.px != INT64_MAX) {
      new_obj->Px(expiration.px);
    } else if (expiration.pxat != INT64_MAX) {
      new_obj->Pxat(expiration.pxat);
    }
    return (flags & GET) ? optional<string>{} : "OK";
  }

  if (flags & NX) {
    if (!(flags & GET)) {
      return {};
    }
    if (old_obj->IsString()) {
      return dynamic_pointer_cast<StringObject>(old_obj)->String();
    }
    status = Status::WrongType();
    return {};
  }

  if (old_obj->IsString()) {
    old_obj->Touch();

    auto string_obj = make_shared<StringObject>(key, expire_helper(), value);
    auto old_string = string_obj->String();
    string_obj->Update(value);

    if (expiration.px != INT64_MAX) {
      string_obj->Px(expiration.px);
    } else if (expiration.pxat != INT64_MAX) {
      string_obj->Pxat(expiration.pxat);
    } else if (!(flags & KEEPTTL) && string_obj->HasExpire()) {
      string_obj->Persist();
    }

    return (flags & GET) ? std::move(old_string) : "OK";
  }

  if (flags & GET) {
    status = Status::WrongType();
    return {};
  }

  DelObject(key);
  auto string_obj = make_shared<StringObject>(key, expire_helper(), value);
  PutObject(key, string_obj);
  if (expiration.px != INT64_MAX) {
    string_obj->Px(expiration.px);
  } else if (expiration.pxat != INT64_MAX) {
    string_obj->Pxat(expiration.pxat);
  }
  return "OK";
}

}  // namespace libcache::db
