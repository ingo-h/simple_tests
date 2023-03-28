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


#if 0
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
