#include <userver/utest/utest.hpp>

#include <atomic>
#include <type_traits>

#include <userver/engine/mutex.hpp>
#include <userver/utils/async.hpp>

UTEST(TestCaseMacros, UTEST_Engine) {
  EXPECT_EQ(GetThreadCount(), 1);

  engine::Mutex mutex;
  std::lock_guard lock(mutex);
}

namespace {

void DeadlockUnlessMultiThreaded() {
  std::atomic<bool> keep_running1{true};
  std::atomic<bool> keep_running2{true};

  auto task1 = utils::Async("task1", [&] {
    keep_running2 = false;
    while (keep_running1) {
    }
  });
  auto task2 = utils::Async("task2", [&] {
    keep_running1 = false;
    while (keep_running2) {
    }
  });

  task1.Get();
  task2.Get();
}

}  // namespace

UTEST_MT(TestCaseMacros, MultiThreaded, 2) {
  EXPECT_EQ(GetThreadCount(), 2);
  DeadlockUnlessMultiThreaded();
}

class TestCaseMacrosFixture : public ::testing::Test {
 public:
  TestCaseMacrosFixture() { CheckEngine(); }

  ~TestCaseMacrosFixture() override { CheckEngine(); }

 protected:
  void SetUp() override { CheckEngine(); }

  void TearDown() override { CheckEngine(); }

  void CheckEngine() { std::lock_guard lock(mutex_); }

  engine::Mutex mutex_;
};

UTEST_F(TestCaseMacrosFixture, UTEST_F_Engine) { CheckEngine(); }

UTEST_F_MT(TestCaseMacrosFixture, UTEST_F_Engine2, 2) {
  EXPECT_EQ(GetThreadCount(), 2);
  DeadlockUnlessMultiThreaded();
}

class TestCaseMacrosParametric : public ::testing::TestWithParam<std::string> {
 public:
  static_assert(std::is_same_v<ParamType, std::string>);

  TestCaseMacrosParametric() { CheckEngineAndParam(); }

  ~TestCaseMacrosParametric() override { CheckEngineAndParam(); }

 protected:
  void SetUp() override { CheckEngineAndParam(); }

  void TearDown() override { CheckEngineAndParam(); }

  void CheckEngineAndParam() {
    std::lock_guard lock(mutex_);
    EXPECT_TRUE(GetParam() == "foo" || GetParam() == "bar");
  }

  engine::Mutex mutex_;
};

UTEST_P(TestCaseMacrosParametric, UTEST_P_Engine) { CheckEngineAndParam(); }

UTEST_P_MT(TestCaseMacrosParametric, UTEST_P_Engine2, 2) {
  CheckEngineAndParam();
  EXPECT_EQ(GetThreadCount(), 2);
  DeadlockUnlessMultiThreaded();
}

INSTANTIATE_UTEST_SUITE_P(FooBar, TestCaseMacrosParametric,
                          testing::Values("foo", "bar"));

class TestCaseMacrosParametric2 : public TestCaseMacrosParametric {};

INSTANTIATE_UTEST_SUITE_P(TestCaseMacrosParametric2, TestCaseMacrosParametric2,
                          testing::Values("foo"));

UTEST_P(TestCaseMacrosParametric2, InstantiatedBeforeTest) {}

template <typename T>
class TestCaseMacrosTyped : public TestCaseMacrosFixture {
 protected:
  T GetTypeInstance() { return T{}; }
};

using MyTypes = ::testing::Types<char, bool, std::string>;

TYPED_UTEST_SUITE(TestCaseMacrosTyped, MyTypes);

TYPED_UTEST(TestCaseMacrosTyped, TYPED_UTEST_Engine) {
  this->CheckEngine();
  static_assert(std::is_same_v<decltype(this->GetTypeInstance()), TypeParam>);
  static_assert(std::is_same_v<TestFixture, TestCaseMacrosTyped<TypeParam>>);
}

TYPED_UTEST_MT(TestCaseMacrosTyped, TYPED_UTEST_Engine2, 2) {
  this->CheckEngine();
  EXPECT_EQ(GetThreadCount(), 2);
  DeadlockUnlessMultiThreaded();
}

template <typename T>
class TestCaseMacrosTypedP : public TestCaseMacrosFixture {
 protected:
  T GetTypeInstance() { return T{}; }
};

TYPED_UTEST_SUITE_P(TestCaseMacrosTypedP);

TYPED_UTEST_P(TestCaseMacrosTypedP, TYPED_UTEST_P_Engine) {
  this->CheckEngine();
  static_assert(std::is_same_v<decltype(this->GetTypeInstance()), TypeParam>);
  static_assert(std::is_same_v<TestFixture, TestCaseMacrosTypedP<TypeParam>>);
}

TYPED_UTEST_P_MT(TestCaseMacrosTypedP, TYPED_UTEST_P_Engine2, 2) {
  this->CheckEngine();
  EXPECT_EQ(GetThreadCount(), 2);
  DeadlockUnlessMultiThreaded();
}

REGISTER_TYPED_UTEST_SUITE_P(TestCaseMacrosTypedP, TYPED_UTEST_P_Engine,
                             TYPED_UTEST_P_Engine2);

INSTANTIATE_TYPED_UTEST_SUITE_P(MyTypes, TestCaseMacrosTypedP, MyTypes);