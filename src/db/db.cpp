#include "db.hpp"

using libcache::expire::SteadyTimePoint;
using libcache::expire::SystemTimePoint;
using libcache::expire::TimePoint;
using std::lock_guard;
using std::make_shared;
using std::mutex;
using std::shared_ptr;
using std::string;

namespace libcache::db {

DB::~DB() {
  lock_guard<mutex> lock(mutex_);
  for (auto& [_, obj] : objects_) {
    if (obj->expire_at()) {
      RemoveExpire(obj);
    }
  }
}

void DB::Tick() {
  lock_guard<mutex> lock(mutex_);
  system_tw_.Tick();
  steady_tw_.Tick();
}

shared_ptr<Object> DB::GetObject(const string& key) const {
  auto it = objects_.find(key);
  if (it == objects_.end()) {
    return nullptr;
  }

  auto obj = it->second;
  auto expire_at = obj->expire_at();
  if (!expire_at) {
    return obj;
  }

  int64_t pttl = expire_at->Pttl();
  if (pttl <= 0) {
    return nullptr;
  }

  return obj;
}

void DB::PutObject(const string& key, shared_ptr<Object> obj) {
  assert(!HasObject(key));
  objects_.insert({key, obj});
}

void DB::DelObject(const string& key) {
  assert(HasObject(key));

  auto obj = objects_.at(key);
  if (obj->expire_at()) {
    RemoveExpire(obj);
  }
  objects_.erase(key);
}

void DB::ExpireAfterMsec(const string& key, int64_t msec) {
  assert(msec > 0);
  assert(HasObject(key));

  auto obj = objects_.at(key);
  if (obj->expire_at()) {
    RemoveExpire(obj);
  }

  auto tp = steady_tw_.Add(
      SteadyTimePoint::AfterNowMsec(msec),
      [this, key = key /* 拷贝一份，防止悬垂引用 */]() { OnExpire(key); });
  auto expire_at = make_shared<SteadyTimePoint>(std::move(tp));
  obj->set_expire_at(expire_at);
}

void DB::ExpireAtUnixMsec(const string& key, int64_t msec) {
  assert(msec > 0);
  assert(HasObject(key));

  auto obj = objects_.at(key);
  if (obj->expire_at()) {
    RemoveExpire(obj);
  }

  auto tp = system_tw_.Add(
      SystemTimePoint::FromUnixMsec(msec),
      [this, key = key /* 拷贝一份，防止悬垂引用 */]() { OnExpire(key); });
  auto expire_at = make_shared<SystemTimePoint>(std::move(tp));
  obj->set_expire_at(expire_at);
}

void DB::RemoveExpire(shared_ptr<Object> obj) {
  auto expire_at = obj->expire_at();
  assert(expire_at);

  if (expire_at->type() == TimePoint::Type::kSystem) {
    system_tw_.Remove(*expire_at->ToSystem());
  } else {
    steady_tw_.Remove(*expire_at->ToSteady());
  }
  obj->set_expire_at(nullptr);
}

void DB::OnExpire(const string& key) {
  auto it = objects_.find(key);
  assert(it != objects_.end());
  auto obj = it->second;
  obj->set_expire_at(nullptr);
  objects_.erase(it);
}

}  // namespace libcache::db
