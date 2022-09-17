// Test with mocked object as parameter to caller program,
// means dependency injection.
// ./compile.sh proof*.cpp [-DMOCK]
// Author: 2021-03-01 Ingo Höft <Ingo@Hoeft-online.de>, last modified 2022-09-16

#include "gmock/gmock.h"

class WorkerInterface {
  public:
    virtual ~WorkerInterface() {}
    virtual int working() = 0;
};

//
class Worker : public WorkerInterface {
  public:
    virtual ~Worker() override {}
    int working() { return 1; }
};

//
class Caller {
  public:
    Caller(WorkerInterface* a_WorkerObj) : m_WorkObj(a_WorkerObj) {}

    int calling() {
        int ret = m_WorkObj->working();
        std::cout << "workerObj.working returned = " << ret << "\n";
        return ret;
    }

  private:
    WorkerInterface* m_WorkObj;
};

//
#ifndef MOCK
int main(int argc, char** argv) {
    Worker workerObj;
    Caller callerObj(&workerObj);
    return (callerObj.calling() == 1 ? 0 : 1);
}
#else

//
class WorkerMock : public WorkerInterface {
  public:
    virtual ~WorkerMock() override {}
    MOCK_METHOD(int, working, ());
};

TEST(MockTestSuite, working) {
    WorkerMock workerMockObj;
    // Using default expectations
    EXPECT_CALL(workerMockObj, working());

    // Inject worker object
    Caller callerObj(&workerMockObj);
    EXPECT_EQ(callerObj.calling(), 0);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
#endif
