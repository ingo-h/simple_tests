// With helpful information from
// REF: [Example: Accepting connections from both IPv6 and IPv4 clients]
// (https://www.ibm.com/docs/en/i/7.6.0?topic=sscaaiic-example-accepting-connections-from-both-ipv6-ipv4-clients)

#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        // clang-format off
        std::cerr << "Usage:   " << argv[0] << " <port>\n"
                     "Example: " << argv[0] << " 50000\n"
                     "Then you can use 'nc (netcat)' on a remote client to "
                     "establish a connection,\n"
                     "e.g.: nc 192.168.24.88 50000\n";
        // clang-format on
        return -1;
    }

    // Get address info for passive listening on all local network interfaces.
    addrinfo hints{}, *res{nullptr};
    hints.ai_family = AF_INET6;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE | AI_NUMERICHOST | AI_NUMERICSERV;
    int rc = ::getaddrinfo(nullptr, argv[1], &hints, &res);
    if (rc != 0) {
        std::cerr << "Error: getaddrinfo() fails with code=" << rc << " - "
                  << ::gai_strerror(rc) << '\n';
        exit(1);
    }
    if (res->ai_next != nullptr) {
        std::cerr << "Error: getaddrinfo() fails with more than one address "
                     "info. Expected is only one.\n";
        exit(1);
    }

    // Get socket file descriptor, using address info.
    int sfd = ::socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sfd == -1) {
        std::cerr << "Error: socket() fails with errno=" << errno << " - "
                  << ::strerror(errno) << '\n';
        exit(1);
    }

    // Set IPV6_V6ONLY to false (is default on Unix platforms but not on
    // Microsoft Windows).
    constexpr int off{0};
    rc = ::setsockopt(sfd, IPPROTO_IPV6, IPV6_V6ONLY, &off, sizeof(off));
    if (rc == -1) {
        std::cerr << "Error: setsockopt() IPV6_V6ONLY fails with code=" << rc
                  << " - " << ::strerror(errno) << '\n';
        exit(1);
    }

    // Bind socket to local network addresses, using address info.
    rc = ::bind(sfd, res->ai_addr, res->ai_addrlen);
    if (rc == -1) {
        std::cerr << "Error: bind() fails with errno=" << errno << " - "
                  << ::strerror(errno) << '\n';
        exit(1);
    }

    // I don't need the address info anymore.
    freeaddrinfo(res);

    // Set socket to listen.
    rc = ::listen(sfd, SOMAXCONN);
    if (rc == -1) {
        std::cerr << "Error: listen() fails with errno=" << errno << " - "
                  << ::strerror(errno) << '\n';
        exit(1);
    }

    // The accept() call will block indefinitely waiting for the incoming
    // connection request to arrive from an IPv4 or IPv6 client.
    sockaddr_storage remote_sa{};
    socklen_t socklen{sizeof(remote_sa)}; // May be modified.
    int remote_sfd =
        ::accept(sfd, reinterpret_cast<sockaddr*>(&remote_sa), &socklen);
    if (remote_sfd == -1) {
        std::cerr << "Error: accept() fails with errno=" << errno << " - "
                  << ::strerror(errno) << '\n';
        exit(1);
    }

    // Show remote IP address.
    char addrStr[INET6_ADDRSTRLEN];
    char servStr[NI_MAXSERV];
    rc = ::getnameinfo((sockaddr*)&remote_sa, socklen, addrStr, sizeof(addrStr),
                       servStr, sizeof(servStr),
                       NI_NUMERICHOST | NI_NUMERICSERV);
    std::cout << "Remote IP address is \"[" << addrStr << "]:" << servStr
              << "\".\n";

    close(remote_sfd);
    close(sfd);
}
