#include <gtest/gtest.h>

#include <chrono>
#include <libcache/libcache.hpp>
#include <thread>

using std::chrono::seconds;
using std::this_thread::sleep_for;

namespace libcache {

TEST(TestString, Append) {
  auto cache = Cache::New();

  auto len = cache->Append("key", "hello");
  EXPECT_EQ(len, 5);
  auto result = cache->Get("key");
  EXPECT_EQ(result.value(), "hello");

  len = cache->Append("key", "world");
  EXPECT_EQ(len, 10);
  result = cache->Get("key");
  EXPECT_EQ(result.value(), "helloworld");

  delete cache;
}

TEST(TestString, AppendWithExpire) {
  auto cache = Cache::New();

  auto result = cache->Set("key", "hello", 0, EX(1));
  EXPECT_EQ(result.value(), "OK");
  result = cache->Get("key");
  EXPECT_EQ(result.value(), "hello");

  auto len = cache->Append("key", "world");
  EXPECT_EQ(len, 10);
  result = cache->Get("key");
  EXPECT_EQ(result.value(), "helloworld");

  auto pttl = cache->Pttl("key");
  EXPECT_LE(pttl, 1000);
  EXPECT_GT(pttl, 0);

  delete cache;
}

TEST(TestString, Set) {
  auto cache = Cache::New();

  auto result = cache->Get("key");
  EXPECT_FALSE(result.has_value());

  result = cache->Set("key", "value1");
  EXPECT_EQ(result, "OK");
  result = cache->Get("key");
  EXPECT_EQ(result.value(), "value1");

  result = cache->Set("key", "value2");
  EXPECT_EQ(result, "OK");
  result = cache->Get("key");
  EXPECT_EQ(result.value(), "value2");

  delete cache;
}

TEST(TestString, SetWithNX) {
  auto cache = Cache::New();

  auto result = cache->Get("key");
  EXPECT_FALSE(result.has_value());

  result = cache->Set("key", "value1", NX);
  EXPECT_EQ(result, "OK");
  result = cache->Get("key");
  EXPECT_EQ(result.value(), "value1");

  result = cache->Set("key", "value2", NX);
  EXPECT_FALSE(result.has_value());
  result = cache->Get("key");
  EXPECT_EQ(result.value(), "value1");

  delete cache;
}

TEST(TestString, SetWithXX) {
  auto cache = Cache::New();

  auto result = cache->Get("key");
  EXPECT_FALSE(result.has_value());

  result = cache->Set("key", "value1", XX);
  EXPECT_FALSE(result.has_value());
  result = cache->Get("key");
  EXPECT_FALSE(result.has_value());

  result = cache->Set("key", "value1");
  EXPECT_EQ(result.value(), "OK");
  result = cache->Get("key");
  EXPECT_EQ(result.value(), "value1");

  result = cache->Set("key", "value2", XX);
  EXPECT_EQ(result.value(), "OK");
  result = cache->Get("key");
  EXPECT_EQ(result.value(), "value2");

  delete cache;
}

TEST(TestString, SetWithEX) {
  auto cache = Cache::New();

  auto result = cache->Set("key", "value", 0, EX(1));
  EXPECT_EQ(result.value(), "OK");
  result = cache->Get("key");
  EXPECT_EQ(result.value(), "value");

  auto pttl = cache->Pttl("key");
  EXPECT_LE(pttl, 1000);
  EXPECT_GT(pttl, 0);

  sleep_for(seconds(1));

  result = cache->Get("key");
  EXPECT_FALSE(result.has_value());

  delete cache;
}

TEST(TestString, SetWithPX) {
  auto cache = Cache::New();

  auto result = cache->Set("key", "value", 0, PX(1000));
  EXPECT_EQ(result.value(), "OK");
  result = cache->Get("key");
  EXPECT_EQ(result.value(), "value");

  auto pttl = cache->Pttl("key");
  EXPECT_LE(pttl, 1000);
  EXPECT_GT(pttl, 0);

  sleep_for(seconds(1));

  result = cache->Get("key");
  EXPECT_FALSE(result.has_value());

  delete cache;
}

}  // namespace libcache
