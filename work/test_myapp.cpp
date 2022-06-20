#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <iostream>

class MylibInterface {
public:
    virtual ~MylibInterface() {}
    virtual void func(int) = 0;
};


class Mylib : public MylibInterface {
public:
    virtual ~Mylib() {}
    void func(int val) override {
        std::cout << "mylib.func has parameter: '" << val << "'\n";
        return;
    }
};


class MylibMock : public MylibInterface {
public:
    virtual ~MylibMock() {}
    MOCK_METHOD(void, func, (int), (override));
};


class Myapp {
    // this pointer will be injected by the injector either with pointing
    // to a real object or to a mock object. The interface ensures that both
    // objects have the same method calls.
    MylibInterface* m_mylib;

public:
    Myapp(MylibInterface* mylib) : m_mylib(mylib) {}

    bool func() {
        m_mylib->func(123);
        return true;
    }
};


// Custom action to return the output parameter
struct MultiplyBy {
    template <typename T>
    void operator()(T arg) {
        std::cout << "arg: '" << arg << "', multiplier: '" << multiplier << "'\n";
        arg = 456;
        return;
     }

  int multiplier;
};


TEST(MylibTestSuite, mock_mylib_func)
// this test macro can be seen as the injector. It determins what object
// is injected to myapp.
{
    using ::testing::_;
    using ::testing::Return;
    using ::testing::DoAll;
    using ::testing::SetArgReferee;

    // inject a real mylib object to myapp and exersize it
    Mylib mylib;
    Myapp myapp(&mylib);
    std::cout << "  real ";
    myapp.func();

    // inject a mocked mylib object to myapp
    MylibMock mylib_mock;
    Myapp myapp_mock(&mylib_mock);
    EXPECT_CALL(mylib_mock, func(_))
        .WillOnce(MultiplyBy{7});

    // and exersize it
    std::cout << "mocked ";
    EXPECT_TRUE(myapp_mock.func());
}


int main(int argc, char** argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
