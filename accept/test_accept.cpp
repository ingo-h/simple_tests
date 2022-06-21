// Copyright (C) 2022 GPL 3 and higher by Ingo Höft,  <Ingo@Hoeft-online.de>
// Redistribution only with this Copyright remark. Last modified: 2022-06-21

// Taken from https://stackoverflow.com/q/72662999/5014688
// For more information about this issue and for another solution "SideEffect2"
// look there.
// ----------------------------------------------------------------------------

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "gmock/gmock.h"

using ::testing::DoAll;
using ::testing::Return;
using ::testing::SetArgPointee;

class MockedSysSocket {
  public:
    MOCK_METHOD(int, accept, (int, sockaddr*, socklen_t* addrlen), ());
};

TEST(MockedSysSocketTest, SideEffect1) {
    MockedSysSocket mocked_sys_socket;

    int accept_sock = 3;
    int listen_sock = 4;

    struct sockaddr_storage clientAddr {};
    struct sockaddr* sa{(sockaddr*)&clientAddr};
    struct sockaddr_in* sa_in{(sockaddr_in*)&clientAddr};

    struct sockaddr_storage clientAddrMock;
    struct sockaddr* sa_mock{(sockaddr*)&clientAddrMock};
    struct sockaddr_in* sa_in_mock{(sockaddr_in*)&clientAddrMock};
    sa_in_mock->sin_family = AF_INET;
    sa_in_mock->sin_port = htons(50000);
    inet_pton(AF_INET, "192.168.1.2", &sa_in_mock->sin_addr);

    socklen_t clientLen = sizeof(sockaddr_storage);

    EXPECT_CALL(mocked_sys_socket, accept(listen_sock, sa, &clientLen))
        .WillOnce(DoAll(SetArgPointee<1>(*sa_mock), Return(accept_sock)));

    auto actual = mocked_sys_socket.accept(listen_sock, sa, &clientLen);
    EXPECT_EQ(sa_in->sin_family, AF_INET);
    EXPECT_EQ(sa_in->sin_port, ntohs(50000));
    char text_addr[INET_ADDRSTRLEN];
    EXPECT_NE(
        inet_ntop(AF_INET, &sa_in->sin_addr, text_addr, sizeof(text_addr)),
        nullptr);
    EXPECT_STREQ(text_addr, "192.168.1.2");
    EXPECT_EQ(actual, accept_sock);
}

#if false
TEST(MockedSysSocketTest, SideEffect2) {
  MockedSysSocket mocked_sys_socket;

  int accept_sock = 1;
  int listen_sock = 2;

  struct sockaddr sa_in {
    "initial"
  };
  struct sockaddr sa_next {
    "next"
  };

  socklen_t clientLen = sizeof(sockaddr);

  EXPECT_CALL(mocked_sys_socket, accept(listen_sock, &sa_in, &clientLen))
      .WillOnce(WithArg<1>(Invoke([&sa_next, accept_sock](sockaddr *in) {
        *in = sa_next;
        return accept_sock;
      })));

  auto actual = mocked_sys_socket.accept(listen_sock, &sa_in, &clientLen);
  EXPECT_EQ(sa_in.name, "next");
  EXPECT_EQ(sa_in.name, sa_next.name);
  EXPECT_EQ(actual, accept_sock);
}
#endif
//-----------------------------------------------------------------------------

int main(int argc, char** argv) {
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
