// Copyright (C) 2022 GPL 3 and higher by Ingo Höft,  <Ingo@Hoeft-online.de>
// Redistribution only with this Copyright remark. Last modified: 2022-09-18
//
// The idea is to use a static member variable within the Caller Class to hold
// the pointer to the Worker function. This pointer can be redirected to a mock
// function with a new object of the Caller Class that will restore the old
// pointer with its destructor. Due to the static pointer variable the
// redirection has a global effect on all objects of the Caller Class. That is
// what we want.
//
// For using static member variables look at:
// https://www.learncpp.com/cpp-tutorial/static-member-variables/
//
// test_proof5-work should return  "workerObj.working() returned = 1"
//
// test_proof5-mock should return ("workerObj.working() returned = 1")
//     this is the mocked function "workerObj.working() returned = 0"
//     this is the mocked function "workerObj.working() returned = 0"
//                                ("workerObj.working() returned = 1")

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
    // This constructor is used to inject the pointer to the mocking function.
    Caller(WorkerInterface* a_ptr_mockObj) {
        m_ptr_oldObj = m_ptr_workerObj;
        m_ptr_workerObj = a_ptr_mockObj;
    }

    // This constructor is used to call the set() method and save the pointer to
    // the real function.
    Caller() = default;

    // The destructor is ussed to restore the old pointer.
    virtual ~Caller() { m_ptr_workerObj = m_ptr_oldObj; }

    void set(WorkerInterface* a_ptr_realObj) {
        m_ptr_workerObj = a_ptr_realObj;
    }

    int calling() {
        int ret = m_ptr_workerObj->working();
        std::cout << "workerObj.working() returned = " << ret << "\n";
        return ret;
    }

  private:
    // Must be static to be also available on a new constructed object.
    // With inline we do not need an extra definiton line outside the class.
    static inline WorkerInterface* m_ptr_workerObj{};
    WorkerInterface* m_ptr_oldObj{};
};
// Not needed with inline declaration
// WorkerInterface* Caller::m_ptr_workerObj; // Definition, initialized with 0

//
#ifndef MOCK
int main(int argc, char** argv) {
    Worker workerObj;

    // Inject the pointer to the real object into the caller.
    Caller callerObj;
    callerObj.set(&workerObj);
    return callerObj.calling() == 1 ? 0 : 1;
}
#else

class WorkerMock : public WorkerInterface {
  public:
    virtual ~WorkerMock() override {}
    MOCK_METHOD(int, working, ());
};

TEST(MockTestSuite, working) {
    // Initialize caller with default real worker.
    // This can be done only one time at program start.
    Worker workerObj;
    Caller callerObj;
    callerObj.set(&workerObj);
    EXPECT_EQ(callerObj.calling(), 1);

    { // Scope for the mocking classes

        // Inject the pointer to the mocking object into the caller.
        WorkerMock workerMockObj;
        Caller callerMockObj(&workerMockObj);

        // Use default expectations
        EXPECT_CALL(workerMockObj, working()).Times(2);

        // Test Unit, should return from the mocked function.
        EXPECT_EQ(callerMockObj.calling(), 0);
        // Check global effect on all objects of the Caller Class.
        EXPECT_EQ(callerObj.calling(), 0);

    } // End scope mocking classes. All objects within the block are destructed.

    // The callerObj should now return from the real function again.
    EXPECT_EQ(callerObj.calling(), 1);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
#endif
