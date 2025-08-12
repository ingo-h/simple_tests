#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <iostream>

int main() {

    // For this example I use the SSDP multicast group "239.255.255.250:1900".
    // Prepare socket address as destination address for the group.
    sockaddr_in mcast_group4{};
    mcast_group4.sin_family = AF_INET;
    mcast_group4.sin_port = htons(1900);
    int rc = inet_pton(mcast_group4.sin_family, "239.255.255.250",
                       &mcast_group4.sin_addr);
    if (rc != 1) {
        std::cerr << "Error: inet_pton() fails with code=" << rc
                  << " - no valid network address or address family given.\n";
        exit(1);
    }

    // Get address info for passive listening on all local network interfaces.
    addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
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
    int sockfd4 = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd4 == -1) {
        std::cerr << "Error: socket() fails with errno=" << errno << " - "
                  << strerror(errno) << '\n';
        exit(1);
    }

    // Bind socket to local network addresses, using address info.
    rc = bind(sockfd4, res->ai_addr, res->ai_addrlen);
    if (rc == -1) {
        std::cerr << "Error: bind() fails with errno=" << errno << " - "
                  << strerror(errno) << '\n';
        exit(1);
    }

    // I don't need the address info anymore.
    freeaddrinfo(res);

    // Send data.
    ssize_t bytes_sent = sendto(
        sockfd4, "Hello world", 12, 0,
        reinterpret_cast<const sockaddr*>(&mcast_group4), sizeof(mcast_group4));
    if (bytes_sent == -1) {
        std::cerr << "Error: sendto() fails with errno=" << errno << " - "
                  << strerror(errno) << '\n';
        exit(1);
    }

    close(sockfd4);
    std::cout << "Works!\n";
    return 0;
}
