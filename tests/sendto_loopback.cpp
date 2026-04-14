#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>

int main() {

    int sock = ::socket(AF_INET6, SOCK_DGRAM, 0);
    char buf[] = "ShutDown";
    sockaddr_in6 saddr{};
    saddr.sin6_family = AF_INET6;
    inet_pton(AF_INET6, "::1", &saddr.sin6_addr);
    saddr.sin6_port = htons(50000);

    ssize_t ret =
        sendto(sock, buf, sizeof(buf) - 1, 0, (sockaddr*)&saddr, sizeof(saddr));
    if (ret == -1)
        std::cerr << "sendto errno=" << errno << "\n";
    else
        std::cerr << "ret=" << ret << "\n";

    return 0;
}
