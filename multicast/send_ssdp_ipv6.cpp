#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <iostream>
#ifdef __APPLE__
#include <net/if.h>
#endif

int main() {

    // For this example I use the SSDP multicast group "[ff02::c]:1900".
    // Prepare socket address as destination address for the group.
    sockaddr_in6 mcast_group6{};
    mcast_group6.sin6_family = AF_INET6;
    mcast_group6.sin6_port = htons(1900);
    int rc =
        inet_pton(mcast_group6.sin6_family, "ff02::c", &mcast_group6.sin6_addr);
    if (rc != 1) {
        std::cerr << "Error: inet_pton() fails with code=" << rc
                  << " - no valid network address or address family given.\n";
        exit(1);
    }
#ifdef __APPLE__
    // Special need for MacOS: sendto() fails if it doesn't have the scope id
    // in the destination multicast group socket address. I guess for this
    // simple test that it uses interface "en0" for sending. The Github Action
    // for this test has a step "show local interface addresses" where you can
    // see the real used interface if this fails.
    mcast_group6.sin6_scope_id = if_nametoindex("en0");
    if (mcast_group6.sin6_scope_id == 0) {
        std::cerr << "Error: nametoindex() fails with errno=" << errno << " - "
                  << strerror(errno) << '\n';
        exit(1);
    }
#endif

    // Get address info for passive listening on all local network interfaces.
    addrinfo hints{}, *res{nullptr};
    hints.ai_family = AF_INET6;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE | AI_NUMERICHOST | AI_NUMERICSERV;
    rc = getaddrinfo(nullptr, "1900", &hints, &res);
    if (rc != 0) {
        std::cerr << "Error: getaddrinfo() fails with code=" << rc << " - "
                  << gai_strerror(rc) << '\n';
        exit(1);
    }
    if (res->ai_next != nullptr) {
        std::cerr << "Error: getaddrinfo() fails with more than one address "
                     "info. Expected is only one.\n";
        exit(1);
    }

    // Get socket file descriptor, using address info.
    int sockfd6 = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd6 == -1) {
        std::cerr << "Error: socket() fails with errno=" << errno << " - "
                  << strerror(errno) << '\n';
        exit(1);
    }

    // Bind socket to local network addresses, using address info.
    rc = bind(sockfd6, res->ai_addr, res->ai_addrlen);
    if (rc == -1) {
        std::cerr << "Error: bind() fails with errno=" << errno << " - "
                  << strerror(errno) << '\n';
        exit(1);
    }

    // I don't need the address info anymore.
    freeaddrinfo(res);

    // Send data.
    ssize_t bytes_sent = sendto(
        sockfd6, "Hello world", 12, 0,
        reinterpret_cast<const sockaddr*>(&mcast_group6), sizeof(mcast_group6));
    if (bytes_sent == -1) {
        std::cerr << "Error: sendto() fails with errno=" << errno << " - "
                  << strerror(errno) << '\n';
        exit(1);
    }

    close(sockfd6);
    std::cout << "Works!\n";
    return 0;
}
