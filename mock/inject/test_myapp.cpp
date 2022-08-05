#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <iostream>

class MylibInterface {
public:
    virtual ~MylibInterface() {}
    virtual int func() = 0;
};


class Mylib : public MylibInterface {
public:
    virtual ~Mylib() {}
    int func() override {
        return 123;
    }
};


class MylibMock : public MylibInterface {
public:
    virtual ~MylibMock() {}
    MOCK_METHOD(int, func, (), (override));
};


class Myapp {
    // this pointer will be injected by the injector either with pointing
    // to a real object or to a mock object. The interface ensures that both
    // objects have the same method calls.
    MylibInterface* m_mylib;

public:
    Myapp(MylibInterface* mylib) : m_mylib(mylib) {}

    bool func() {
        int ret = m_mylib->func();
        std::cout << "mylib.func returns: '" << ret << "'\n";
        return true;
    }
};


TEST(MylibTestSuite, mock_mylib_func)
// this test macro can be seen as the injector. It determins what object
// is injected to myapp.
{
    using ::testing::Return;

    // inject a real mylib object to myapp and exersize it
    Mylib mylib;
    Myapp myapp(&mylib);
    std::cout << "  real ";
    EXPECT_TRUE(myapp.func());

    // inject a mocked mylib object to myapp
    MylibMock mylib_mock;
    Myapp myapp_mock(&mylib_mock);
    EXPECT_CALL(mylib_mock, func())
        .WillOnce(Return(456));

    // and exersize it
    std::cout << "mocked ";
    EXPECT_TRUE(myapp_mock.func());
}


int main(int argc, char** argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
