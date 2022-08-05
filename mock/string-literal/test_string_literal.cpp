// Copyright (C) 2022 GPL 3 and higher by Ingo Höft,  <Ingo@Hoeft-online.de>
// Redistribution only with this Copyright remark. Last modified: 2022-06-24
// ----------------------------------------------------------------------------

#include "gmock/gmock.h"

using ::testing::_;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::SetArgPointee;

int get_string(char* a_buf) {
    strcpy(a_buf, "Hi");
    return 0;
}

// Interface
class WrapperInterface {
  public:
    virtual ~WrapperInterface() {}
    virtual int get_string(char* buf) = 0;
};

// Version to call the real system function.
class RealWrapper : public WrapperInterface {
  public:
    virtual ~RealWrapper() override {}
    int get_string(char* buf) override { return ::get_string(buf); }
};

// Version to call the mocked system function.
class MockWrapper : public WrapperInterface {
  public:
    virtual ~MockWrapper() override {}
    MOCK_METHOD(int, get_string, (char* buf), (override));
};

class Wrapper {
    WrapperInterface* m_wrapper;

  public:
    Wrapper(WrapperInterface* a_wrapper) : m_wrapper(a_wrapper) {}

    void get() {
        char buffer[16]{};

        int ret = m_wrapper->get_string(buffer);
        std::cout << "buffer = '" << buffer << "'\n";
    }
};

//
// Custom action to return a string literal
// Source: https://groups.google.com/g/googlemock/c/lQqCMW1ANQA
// simple version: ACTION_P(StrCpyToArg0, str) { strcpy(arg0, str); }
ACTION_TEMPLATE(StrCpyToArg, HAS_1_TEMPLATE_PARAMS(int, k),
                AND_1_VALUE_PARAMS(str)) {
    strcpy(std::get<k>(args), str);
}

TEST(MockStringLiteralTestSuite, real_wrapper) {
    RealWrapper real_wrapper;
    // Inject real sys_socket object
    Wrapper wrapper(&real_wrapper);

    // Test Unit
    wrapper.get();
}

TEST(MockStringLiteralTestSuite, mock_wrapper) {
    MockWrapper mocked_wrapper;
    // Inject mocked sys_socket object
    Wrapper wrapper(&mocked_wrapper);

    EXPECT_CALL(mocked_wrapper, get_string(_))
        .WillOnce(DoAll(StrCpyToArg<0>("Hello"), Return(0)));

    // Test Unit
    wrapper.get();
}
//-----------------------------------------------------------------------------

int main(int argc, char** argv) {
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
