// Test with global pointer to the mocked object.
// That is what we need to mock a global free function from a C library.
// ./compile.sh proof*.cpp [-DMOCK]
// Author: 2021-03-01 Ingo Höft <Ingo@Hoeft-online.de>, last modified 2022-09-16

#include "gmock/gmock.h"

class WorkerInterface {
  public:
    virtual ~WorkerInterface() {}
    virtual int working() = 0;
};

// Global pointer that points either to the real object or to the mocked object.
WorkerInterface* ptrWorkerObj{};

//
class Worker : public WorkerInterface {
  public:
    virtual ~Worker() override {}
    int working() { return 0; }
};

//
class Caller {
  public:
    int calling() {
        int ret = ptrWorkerObj->working();
        std::cout << "workerObj.working() returned = " << ret << "\n";
        return ret;
    }
};

#ifndef MOCK
int main(int argc, char** argv) {
    Worker workerObj;
    // Set the global pointer to the real object.
    ptrWorkerObj = &workerObj;

    Caller callerObj;
    return callerObj.calling();
}
#else

class WorkerMock : public WorkerInterface {
  public:
    virtual ~WorkerMock() override {}
    MOCK_METHOD(int, working, ());
};

TEST(MockTestSuite, working) {
    WorkerMock workerMockObj;
    // Set the global pointer to the mocked object.
    ptrWorkerObj = &workerMockObj;

    // Use default expectations
    EXPECT_CALL(workerMockObj, working());

    Caller callerObj;
    EXPECT_EQ(callerObj.calling(), 0);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
#endif
