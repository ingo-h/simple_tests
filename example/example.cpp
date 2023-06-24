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

struct addrinfo hint {};
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
            std::cout << address_sr << " AF_INET6 = [" << addr_buf
                      << "]:" << ntohs(addr.s6.sin6_port) << "\n";
        } break;

        default:
            std::cerr << "ERROR! invalid address family " << addr.ss.ss_family
                      << ".\n";
            return 1;
        } // switch
    }     // for

    freeaddrinfo(addrs);
    return 0;
}

#if 0
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
