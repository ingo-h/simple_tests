// Copyright (C) 2022+ GPL 3 and higher by Ingo Höft, <Ingo@Hoeft-online.de>
// Redistribution only with this Copyright remark. Last modified: 2024-04-04

#include <memory>
#include <gmock/gmock.h>

namespace utest {

using ::testing::Return;


class CFooInterface {
  public:
    virtual ~CFooInterface() = default;
    virtual int foo_method() const = 0;
};

using CFooPtr = std::shared_ptr<CFooInterface>;

// This "productive" di-service is used by default.
class CFoo : public CFooInterface {
  public:
    virtual ~CFoo() = default;
    int foo_method() const override {
        return ~0; // This cast to bool true;
    }
};

// This di-service will be injected instead of the "productive" di-service to
// mock the real object for testing.
class CFooMock : public CFooInterface {
  public:
    virtual ~CFooMock() = default;
    MOCK_METHOD(int, foo_method, (), (const override));
};

class CBarRawPtr {
  public:
    CBarRawPtr(CFooInterface* a_fooObj) : m_fooObj(a_fooObj) {}
    virtual ~CBarRawPtr() = default;
    bool success() { //
        return m_fooObj->foo_method();
    }

  private:
    CFooInterface* m_fooObj;
};

// This is the di-client into the di-services are injected. For injection I use
// smart pointers. To get ownership for responsibility and destruction I move
// the service into this object.
class CBarSmartPtr {
  public:
    CBarSmartPtr(CFooPtr a_fooObj)
        : m_fooObj(a_fooObj) {}
    virtual ~CBarSmartPtr() = default;
    // Methods
    bool success() { //
        return m_fooObj->foo_method();
    }

  private:
    CFooPtr m_fooObj;
};

TEST(FooTestSuite, productive_foo_with_raw_pointer) {
    CFoo fooObj;

    // Test Unit
    CBarRawPtr barObj(&fooObj);
    EXPECT_TRUE(barObj.success());
}

TEST(FooTestSuite, mock_foo_with_raw_pointer) {
    CFooMock fooObj;
    // This mocks to CBarRawPtr::success == false
    EXPECT_CALL(fooObj, foo_method()).WillOnce(Return(0));

    // Test Unit
    CBarRawPtr barObj(&fooObj);
    EXPECT_FALSE(barObj.success());
}

TEST(FooTestSuite, productive_foo_with_shared_pointer) {
    auto fooObj = std::make_shared<CFoo>();

    // Test Unit
    CBarSmartPtr barObj(fooObj);
    EXPECT_TRUE(barObj.success());
}

TEST(FooTestSuite, mock_foo_with_shared_pointer) {
    auto fooObj = std::make_shared<CFooMock>();
    // This mocks to CBarSmartPtr::success == false
    EXPECT_CALL(*fooObj, foo_method()).WillOnce(Return(0));

    // Test Unit
    CBarSmartPtr barObj(fooObj);
    EXPECT_FALSE(barObj.success());
}

} // namespace utest


int main(int argc, char** argv) {
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
