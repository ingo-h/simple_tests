// Copyright (C) 2022 GPL 3 and higher by Ingo Höft,  <Ingo@Hoeft-online.de>
// Redistribution only with this Copyright remark. Last modified: 2022-06-21

// Taken from https://stackoverflow.com/q/72662999/5014688
// For more information about this issue and for another solution "SideEffect2"
// look there.
// ----------------------------------------------------------------------------

#include <sys/socket.h>
#include <arpa/inet.h>
#include "gmock/gmock.h"

using ::testing::_;
using ::testing::DoAll;
using ::testing::Pointee;
using ::testing::Return;
using ::testing::SetArgPointee;

//
class SysSocketInterface {
  public:
    virtual ~SysSocketInterface() {}
    virtual int accept(int sockfd, struct sockaddr* addr,
                       socklen_t* addrlen) = 0;
};

// Version to call the real system function.
class RealSysSocket : public SysSocketInterface {
  public:
    virtual ~RealSysSocket() override {}
    int accept(int sockfd, struct sockaddr* addr, socklen_t* addrlen) override {
        return ::accept(sockfd, addr, addrlen);
    }
};

// Version to call the mocked system function.
class MockSysSocket : public SysSocketInterface {
  public:
    virtual ~MockSysSocket() override {}
    MOCK_METHOD(int, accept, (int, struct sockaddr*, socklen_t*), (override));
};

//
class SysSocket {
    SysSocketInterface* m_syssocket;

  public:
    SysSocket(SysSocketInterface* a_syssocket) : m_syssocket(a_syssocket) {}

    std::string get_addr(int a_listen_sock) {
        struct sockaddr_storage clientAddr;
        struct sockaddr_in* clientAddr_in{(sockaddr_in*)&clientAddr};
        socklen_t socklen = sizeof(sockaddr_storage);

        int accept_sock = m_syssocket->accept(a_listen_sock,
                                              (sockaddr*)&clientAddr, &socklen);
        if (accept_sock == -1) {
            return "";
        }

        char text_addr[INET_ADDRSTRLEN];
        if (inet_ntop(AF_INET, &clientAddr_in->sin_addr, text_addr,
                      sizeof(text_addr)) == nullptr) {
            return "";
        }

        unsigned short int port = ntohs(clientAddr_in->sin_port);
        return std::string(text_addr) + ":" + std::to_string(port);
    }
};

//
TEST(SysSocketTestSuite, call_real_sys_socket_accept) {
    RealSysSocket real_sys_socket;
    // Inject real sys_socket object
    SysSocket sys_socket(&real_sys_socket);

    int listen_sock = 0;

    // Test Unit
    // Listen_sock is not a valid socket
    EXPECT_EQ(sys_socket.get_addr(listen_sock), "");
}

TEST(SysSocketTestSuite, mock_sys_socket_accept) {
    // Provide a socket address
    struct sockaddr_storage clientAddrMock;
    struct sockaddr* sa{(sockaddr*)&clientAddrMock};
    struct sockaddr_in* sa_in{(sockaddr_in*)&clientAddrMock};
    sa_in->sin_family = AF_INET;
    sa_in->sin_port = htons(50000u);
    inet_pton(AF_INET, "192.168.1.2", &sa_in->sin_addr);
    socklen_t socklen = sizeof(sockaddr_storage);

    // Instantiate mocked sys_socket object
    MockSysSocket mock_sys_socket;
    // Inject mocked sys_socket object
    SysSocket sys_socket(&mock_sys_socket);

    int listen_sock = 3;
    int accept_sock = 4;

    EXPECT_CALL(mock_sys_socket, accept(listen_sock, _, Pointee(socklen)))
        .WillOnce(DoAll(SetArgPointee<1>(*sa), Return(accept_sock)));

    // Test Unit
    EXPECT_EQ(sys_socket.get_addr(listen_sock), "192.168.1.2:50000");
}
//-----------------------------------------------------------------------------

int main(int argc, char** argv) {
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
