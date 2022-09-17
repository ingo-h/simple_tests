// Test side effects with mocked object as parameter to caller program
// ./compile.sh proof*.cpp [-DMOCK]
// Author: 2021-03-05 Ingo Höft <Ingo@Hoeft-online.de>, last modified 2022-09-16

#include "gmock/gmock.h"

class WorkerInterface {
  public:
    virtual ~WorkerInterface() {}
    virtual int working(int*) = 0;
};

//
class Worker : public WorkerInterface {
  public:
    virtual ~Worker() override {}
    int working(int* setparm) override {
        *setparm = 123;
        return 0;
    }
};

//
class Caller {
  public:
    Caller(WorkerInterface* a_WorkerObj) : m_WorkObj(a_WorkerObj) {}

    int calling() {
        int param;
        int ret = m_WorkObj->working(&param);
        std::cout << "workerObj.working returned = " << ret
                  << " and param = " << param << "\n";
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
    return callerObj.calling();
}
#else

//
class WorkerMock : public WorkerInterface {
  public:
    virtual ~WorkerMock() override {}
    MOCK_METHOD(int, working, (int*), (override));
};

//
TEST(MockTestSuite, working) {
    using ::testing::_;
    using ::testing::DoAll;
    using ::testing::Return;
    using ::testing::SetArgPointee;

    WorkerMock workerMockObj;
    EXPECT_CALL(workerMockObj, working(_))
        .WillOnce(DoAll(SetArgPointee<0>(247), Return(1)));

    Caller callerObj(&workerMockObj);
    EXPECT_EQ(callerObj.calling(), 1);
}

//
int main(int argc, char** argv) {
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
#endif
