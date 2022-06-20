// Mock binding a network interface to a socket
// ./compile_all.sh
// Author: 2021-03-10 - Ingo Höft <Ingo@Hoeft-online.de>

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <netinet/in.h>
#include <arpa/inet.h>
//#include "../../tools/ifaddrs.cpp"

extern "C" {
    int mysocket();
}

// --- mock bind -------------------------------------------
class BindInterface {
public:
    virtual ~BindInterface() {}
    virtual int bind(int sockfd, const struct sockaddr* addr, socklen_t addrlen) = 0;
};

class BindMock : public BindInterface {
public:
    virtual ~BindMock() {}
    MOCK_METHOD(int, bind, (int, const struct sockaddr*, socklen_t), (override));
};

BindInterface* ptrBindMockObj = nullptr;
int bind(int sockfd, const struct sockaddr* addr, socklen_t addrlen) {
    return ptrBindMockObj->bind(sockfd, addr, addrlen);
}

ACTION_P(AssignSockAddr, param) { *(sockaddr*)(arg0) = param; }

TEST(MockTestSuite, bind_address_to_a_socket)
{
    using ::testing::_;
    using ::testing::Return;
    using ::testing::DoAll;
    using ::testing::SetArgPointee;
    using ::testing::SetArgReferee;
    using ::testing::WithArg;

    struct sockaddr_in my_addr = {0};
    //struct sockaddr* ptrMy_addr = (struct sockaddr*)&my_addr;

    BindMock bindMockObj;
    ptrBindMockObj = &bindMockObj;

    my_addr.sin_family = AF_INET;
    EXPECT_EQ(inet_pton(AF_INET, "192.168.55.66", &my_addr.sin_addr.s_addr), 1);

    EXPECT_CALL(bindMockObj, bind(_, _, _))
        .WillOnce(DoAll(WithArg<1>(AssignSockAddr(my_addr)),
                        SetArgReferee<2>(sizeof(my_addr)),
                        Return(0)));

//    EXPECT_CALL(bindMockObj, bind(_, _, _))
//        .WillOnce(DoAll(SetArgPointee<1>((struct sockaddr*)my_addr),
//                        SetArgReferee<2>(sizeof(my_addr)),
//                        Return(0)));

    EXPECT_EQ(mysocket(), EXIT_SUCCESS);
}


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
