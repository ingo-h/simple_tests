// Examine behavior of systemcall ::connect() with IPv4 mapped IPv6 addresses.
// ---------------------------------------------------------------------------
// Compiled with: g++ -std=c++23 -pedantic-errors -Wall connect_write.cpp
//
// Result: The function does not support AI_V4MAPPED local network adapter
// addresses, e.g. "[::ffff:192.168.24.10]". This means that once switched over,
// the system will only support genuine IPv6 addresses.

#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string.h>

int main() {

    // Get socket
    // ----------
    int sockfd = socket(AF_INET6, SOCK_STREAM, 0);

    // Bind socket to local interface address
    // --------------------------------------
    sockaddr_in6 local_sa{};
    local_sa.sin6_family = AF_INET6;
    // Works with valid local network adapter address:
    // char loc_addr[] = "2003:d5:2730:b300:5054:ff:fe7f:c021";
    // Fails:
    char loc_addr[] = "::ffff:192.168.24.88";
    inet_pton(AF_INET6, loc_addr, &local_sa.sin6_addr);

    bind(sockfd, reinterpret_cast<sockaddr*>(&local_sa), sizeof(sockaddr_in6));

    // Get address info for remate network address
    // -------------------------------------------
    addrinfo hints{}, *res{};
    hints.ai_flags = AI_V4MAPPED;
    hints.ai_family = AF_INET6;
    hints.ai_socktype = SOCK_STREAM;

    getaddrinfo("example.com", "http", &hints, &res);

    // Connect to remote host
    // ----------------------
    int ret = connect(sockfd, reinterpret_cast<sockaddr*>(res->ai_addr),
                      res->ai_addrlen);
    if (ret != 0) {
        std::cerr << "Error(" << errno << ") with ::connect() from local \"["
                  << loc_addr << "]\" - " << ::strerror(errno) << "\n";
        close(sockfd);
        return 1;
    }

    // Write request to remote host
    // ----------------------------
    constexpr char wbuf[]{"GET / HTTP/1.1\r\nHost: example.com\r\n\r\n"};
    write(sockfd, wbuf, sizeof(wbuf) - 1);

    // Read response
    // -------------
    char rbuf[2048]{};
    read(sockfd, rbuf, sizeof(rbuf) - 1);
    std::cout << "---------------\n" << rbuf << "---------------\n";

    // Graceful finish
    // ---------------
    shutdown(sockfd, SHUT_RDWR);
    close(sockfd);

    std::cout << "Successful ::connect() from local \"[" << loc_addr << "]\"\n";
    return 0;
}
