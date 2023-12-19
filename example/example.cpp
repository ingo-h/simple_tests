// #############################################################################
// This is to test System error support from the C++ standard library. It is
// confusing what error_code/error_condition to use with what category on which
// platform to use. Here I can test it with modifying different settings. In
// general the error support doesn't really make sense to use.

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h> // for getaddrinfo, socklen_t etc.


int main() {
// We have do initialize win32 sockets
#ifdef _WIN32
    WSADATA wsaData;
    ::WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

    std::error_code ec;

    char so_opt;
    socklen_t optlen{sizeof(so_opt)};
    int rc = getsockopt(55555, SOL_SOCKET, SO_ERROR, &so_opt, &optlen);

    if (rc != 0)
#ifdef _WIN32
        ec = std::error_code(GetLastError(), std::system_category());
#else
        ec = std::error_condition(errno, std::system_category());
#endif

    // To test using portable code
    if (ec == std::errc::bad_file_descriptor)
        std::cout << "match error code 'bad_file_descriptor'(" << ec.value()
                  << ").\n";
    else
        std::cout << "\"" << ec.message() << "\"(" << ec.value()
                  << ") does not match 'bad_file_descriptor'(9).\n";

        // To convert into nearest portable error condition (lossy, may
        // fail)
        // std::error_condition ec2(ec.default_error_condition())

#if _WIN32
    ::WSACleanup();
#endif

    return 0;
}


#if 0
// #############################################################################
#include <netdb.h> // for sockaddr structures
#include <cstring> // for memcmp()
#include <cassert>

using sockaddr_t = union {
    sockaddr_storage ss;
    sockaddr_in6 sin6;
    sockaddr_in sin;
    sockaddr sa;
};

#if false
struct sockaddr_storage {
    sa_family_t      ss_family;
    // Following field(s) are implementation specific
    // only for padding to mostly 128 bytes.
    // No information usable.
};

struct sockaddr_in6 {
    sa_family_t      sin6_family;   // AF_INET6.
    in_port_t        sin6_port;     // Port number.
    uint32_t         sin6_flowinfo; // IPv6 traffic class and flow info.
    struct in6_addr  sin6_addr;     // IPv6 address.
    uint32_t         sin6_scope_id; // Set of interfaces for a scope.
};

struct sockaddr_in {
    sa_family_t      sin_family;    // AF_INET.
    in_port_t        sin_port;      // Port number.
    struct in_addr   sin_addr;      // IP address.
};

struct sockaddr {
    sa_family_t      sa_family;     // Address family.
    // char          sa_data[];     // Socket address (variable-length data).
};

struct in6_addr {
    uint8_t s6_addr[16];
};

struct in_addr {
    in_addr_t s_addr;
};

using sa_family_t = unsigned short int;
using in_addr_t = uint32_t;
using in_port_t = uint16_t;
#endif

// Examples:

int main() {
    sockaddr_t saddr{};

    assert(saddr.ss.ss_family == saddr.sin6.sin6_family);
    assert(saddr.ss.ss_family == saddr.sin.sin_family);
    assert(saddr.ss.ss_family == saddr.sa.sa_family);

    if (saddr.ss.ss_family == AF_INET6) {
        [[maybe_unused]] in_port_t port6 = saddr.sin6.sin6_port;
        [[maybe_unused]] in6_addr addr6 = saddr.sin6.sin6_addr;

        // Check if sin6_addr is null
        uint8_t s6_addr0[16]{};
        [[maybe_unused]] bool res = (memcmp(saddr.sin6.sin6_addr.s6_addr,
                                            s6_addr0, sizeof(s6_addr0)) == 0);
    }

    if (saddr.ss.ss_family == AF_INET) {
        [[maybe_unused]] in_port_t port = saddr.sin.sin_port;
        [[maybe_unused]] in_addr_t addr = saddr.sin.sin_addr.s_addr;
    }

    // sockaddr is only used for type casting on function arguments
    if (saddr.ss.ss_family == AF_INET6) {
        int sockfd = socket(saddr.ss.ss_family, SOCK_DGRAM, 0);
        socklen_t len = sizeof(saddr.sin6);
        [[maybe_unused]] int ret = getsockname(sockfd, (sockaddr*)&saddr, &len);
    }
}


// #############################################################################
#include <netdb.h>
#include <iostream>
#include <cstring>
#include <arpa/inet.h>

typedef union {
    struct sockaddr s;
    struct sockaddr_in s4;
    struct sockaddr_in6 s6;
    struct sockaddr_storage ss;
} address_t;

struct addrinfo hint{};
struct addrinfo *addrs, *serv;
address_t addr;

std::string address_sr{"example.com"}; // Resolves real IP addresses by DNS
std::string s_port{"https"};


int main() {
    hint.ai_family = AF_UNSPEC;
    hint.ai_socktype = SOCK_DGRAM;
    // hint.ai_protocol = IPPROTO_UDP; // Already given by SOCK_DGRAM

    int ret = getaddrinfo(address_sr.c_str(), s_port.c_str(), &hint, &addrs);
    if (ret != 0) {
        std::cerr << "ERROR! " << gai_strerror(ret) << ".\n";
        return 1;
    }

    for (serv = addrs; serv != NULL; serv = serv->ai_next) {
        memcpy(&addr, serv->ai_addr, serv->ai_addrlen);

        char addr_buf[INET6_ADDRSTRLEN]{};
        switch (addr.ss.ss_family) {
        case AF_INET: {
            const char* ret = inet_ntop(AF_INET, &addr.s4.sin_addr, addr_buf,
                                        sizeof(addr_buf));
            if (ret == nullptr) {
                std::cerr << "ERROR! " << std::strerror(errno) << ".\n";
                return 1;
            }
            std::cout << address_sr << " AF_INET  = " << addr_buf << ":"
                      << ntohs(addr.s4.sin_port) << "\n";
        } break;

        case AF_INET6: {
            const char* ret = inet_ntop(AF_INET6, &addr.s6.sin6_addr, addr_buf,
                                        sizeof(addr_buf));
            if (ret == nullptr) {
                std::cerr << "ERROR! " << std::strerror(errno) << ".\n";
                return 1;
            }
            std::cout << address_sr << " AF_INET6 = [" << addr_buf << "]:"
                      << ntohs(addr.s6.sin6_port) << "\n";
        } break;

        default:
            std::cerr << "ERROR! invalid address family " << addr.ss.ss_family << ".\n";
            return 1;
        } // switch
    } // for

    freeaddrinfo(addrs);
    return 0;
}


// #############################################################################
// Answer given at: https://stackoverflow.com/a/76360210/5014688

#include <netinet/in.h>
#include <iostream>

namespace myns {

struct Ssockaddr_storage1 : public ::sockaddr_storage {
    Ssockaddr_storage1() : ::sockaddr_storage() {}
    ~Ssockaddr_storage1() {}
};

// This is only different by using a virtual destructor
struct Ssockaddr_storage2 : public ::sockaddr_storage {
    Ssockaddr_storage2() : ::sockaddr_storage() {}
    virtual ~Ssockaddr_storage2() {}
};

// Not calling the constructor of the base structure
struct Ssockaddr_storage3 : public ::sockaddr_storage {
    Ssockaddr_storage3() {}
    ~Ssockaddr_storage3() {}
};

} // namespace myns

int main() {
    {
        myns::Ssockaddr_storage1 ss;

        std::cout << "Ssockaddr_storage1:\n";
        std::cout << "&ss    = " << &ss
                  << ", ss.ss_family        = " << ss.ss_family << "\n";
        // Direct use of reference works.
        sockaddr_in6* sa_in6 = (sockaddr_in6*)&ss;
        std::cout << "sa_in6 = " << sa_in6
                  << ", sa_in6->sin6_family = " << sa_in6->sin6_family << "\n";
    }
    {
        myns::Ssockaddr_storage2 ss;

        std::cout << "\nSsockaddr_storage2 with virtual destructor:\n";
        std::cout << "&ss    = " << &ss
                  << ", ss.ss_family        = " << ss.ss_family << "\n";
        // Here we have to cast pointer from reference.
        sockaddr_in6* sa_in6 = (sockaddr_in6*)(::sockaddr_storage*)&ss;
        std::cout << "sa_in6 = " << sa_in6
                  << ", sa_in6->sin6_family = " << sa_in6->sin6_family << "\n";
    }
    {
        myns::Ssockaddr_storage3 ss;

        std::cout << "\nSsockaddr_storage3:\n";
        std::cout << "&ss    = " << &ss
                  << ", ss.ss_family        = " << ss.ss_family << "\n";
        sockaddr_in6* sa_in6 = (sockaddr_in6*)(sockaddr_storage*)&ss;
        std::cout << "sa_in6 = " << sa_in6
                  << ", sa_in6->sin6_family = " << sa_in6->sin6_family << "\n";
    }
}


#include <sys/socket.h>

namespace ns1 {
struct sockaddr_storage : public ::sockaddr_storage {
    void set_af(sa_family_t a_af) {
	this->ss_family = a_af;
    }
};
} // namespace ns1

namespace ns1 {
// This compiles and works as expected
void f1() {
    sockaddr_storage ss;
    ss.set_af(AF_INET6);
}
} // namespace ns1

namespace ns2 {
// This also compiles and works without errors
void f2() {
    ns1::sockaddr_storage ss;
    ss.set_af(AF_INET6);
}
} // namespace ns2

namespace ns2 {

using ::ns1::sockaddr_storage;
// With declaring `using` it does not compile
void f3() {
    sockaddr_storage ss;
    ss.set_af(AF_INET6);
}
} // namespace ns2

int main() {}

// #############################################################################
#include <iostream>
#include <thread>
#include <sys/socket.h>
#include <unistd.h>


class CSocket {
public:
CSocket(int domain = 0, int type = 0, int protocol = 0) {
    std::cout << "Construct CSocket\n";
    m_sfd = socket(domain, type, protocol);
}
virtual ~CSocket() {
    std::cout << "Destruct CSocket\n";
    close(m_sfd);
}
int getter() const {
    std::cout << "Executing getter\n";
return m_sfd; }

private:
int m_sfd{-1};
};


class CServer {
public:

CServer() : m_listenObj(AF_INET6, SOCK_STREAM) {
    std::cout << "Doing all stuff for initializing, set options, bind "
		 "and listen.\n";
    // ...
    // bind(m_listenObj.getter(), ai->ai_addr, ai->ai_addrlen);
    // ...
    // listen(m_listenObj.getter(), 1);
    // ...
}

#ifdef OPERATOR_OVERLOAD
void operator()() {
#else
void run() {
#endif
    std::cout << "Doing all stuff to accept and receive messages.\n";
    // ...
    // int connect_sfd = accept(m_listenObj.getter(), nullptr, nullptr);
    // ...
    // recv(connect_sfd, buffer, sizeof(buffer);
    // ...
}

private:
CSocket m_listenObj;
};


int main() {
#ifdef OPERATOR_OVERLOAD
std::thread threadObj( (CServer()) );
#else
CServer serverObj;
std::thread threadObj( &CServer::run, &serverObj);
#endif
threadObj.join();
//std::this_thread::sleep_for(std::chrono::seconds(3));
return 0;
}


#include <netdb.h>
#include <memory>
#include <iostream>

int main() {
// std::unique_ptr<addrinfo, decltype(&freeaddrinfo)> ptr_;

addrinfo hints{};
hints.ai_family = AF_INET6;
hints.ai_socktype = SOCK_STREAM;
hints.ai_flags = AI_PASSIVE | AI_NUMERICSERV;

addrinfo* res{nullptr};

int ret = ::getaddrinfo("::1", "50123", &hints, &res);
if (ret != 0) {
    std::cerr << "Error\n";
    return EXIT_FAILURE;
}

std::unique_ptr<addrinfo, decltype(&freeaddrinfo)> uptr(res, freeaddrinfo);
std::cout << "uptr->family = " << uptr->ai_family << "\n";
return EXIT_SUCCESS;
}


#include <iostream>

using namespace std;
class T1
{
const int t = 100;
public:

T1()
{

cout << "T1 constructor: " << t << endl;
}
};

int main() {
T1 t1;
}


#include <netdb.h>

class Wrapper {
Wrapper() {};

public:
Wrapper(int family)
    : m_hints{{}, family, {}, {}, {}, nullptr, nullptr, nullptr} {}

private:
const addrinfo m_hints{};
};

int main() {
Wrapper wrap(AF_INET6);
return 0;
}
#endif
