#include "gtest/gtest.h"
#include "gmock/gmock.h"

extern "C" {
    int myStrerror();
}


class MockStrerror {
public:
    MOCK_METHOD(char*, strerror, (int));
};

// We need a pointer to the Mock Object here in the global mocked function
// so we can return its result. The Mock Object must be defined later in the
// test body, otherwise we get a memory leak error. As far as the Mock Object
// is defined, we set this pointer to it.
MockStrerror* ptrMockStrerrorObj = nullptr;
char* strerror(int error_number)
{
    std::cout << "mocked strerror function called\n";
    return ptrMockStrerrorObj->strerror(error_number);
}


TEST(MockTestSuite, strerror)
{
    using ::testing::Return;

    char response[] = "mocked strerror function";

    MockStrerror mockStrerrorObj;
    // Set global pointer to this Mock Object.
    ptrMockStrerrorObj = &mockStrerrorObj;

    EXPECT_CALL(mockStrerrorObj, strerror(0))
        .WillOnce(Return(response));

    EXPECT_EQ(myStrerror(), 0);
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
