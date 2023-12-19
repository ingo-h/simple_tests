#include <iostream>

#ifdef _MSC_VER
// This simple test doesn't make sense on Microsoft Windows because it doesn't
// support sigpipe. It doesn't know the signal name SIGPIPE and never raise a
// signal with broken pipe on socket write(). But it compiles the object
// "instantiation" without error to be portable.
#include "sigpipe.hpp"

int main() {
    SCOPED_NO_SIGPIPE
    std::cout << "Nothing done.\n";
}

#else

#include "sigpipe.hpp"

namespace {
volatile std::sig_atomic_t gSignalStatus;
}
void signal_handler(int signal) { gSignalStatus = signal; }

int main() {
    // Install a signal handler
    std::signal(SIGPIPE, signal_handler);

    // Suppress signal
    std::cout << "SignalValue: " << gSignalStatus << '\n';
    std::cout << "Suppress signal SIGPIPE\nRaise SIGPIPE = " << SIGPIPE << '\n';
    {
        SCOPED_NO_SIGPIPE
        if (std::raise(SIGPIPE) != 0)
            return EXIT_FAILURE; // Signal will be restored
    }                            // Signal will be restored

    if (gSignalStatus != 0)
        std::cout << "Signal delivered, signal_handler executed." << '\n';
    std::cout << "SignalValue: " << gSignalStatus << '\n';

    // Not suppressing signal
    std::cout << "\nRaise SIGPIPE = " << SIGPIPE << '\n';
    std::raise(SIGPIPE);
    if (gSignalStatus != 0)
        std::cout << "Signal delivered, signal_handler executed." << '\n';
    std::cout << "SignalValue: " << gSignalStatus << '\n';

    return EXIT_SUCCESS;
}
#endif

#if 0
#include <cerrno>
#include <openssl/ssl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>

int main() {
    // Provide a context structure.
    SSL_CTX* ssl_ctx{};

    // Create connection context
    ssl_ctx = SSL_CTX_new(TLS_method());
    if (ssl_ctx == nullptr) {
        perror("error");
        exit(EXIT_FAILURE);
    }
    // Create structure for a connection
    SSL* ssl = SSL_new(ssl_ctx);
    if (ssl == nullptr) {
        perror("SSL_new");
        SSL_CTX_free(ssl_ctx);
        exit(EXIT_FAILURE);
    }
    // Associate a socket to a ssl connection
    int sockfd = socket(AF_INET6, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        SSL_free(ssl);
        SSL_CTX_free(ssl_ctx);
        exit(EXIT_FAILURE);
    }
    if (SSL_set_fd(ssl, sockfd) == 0) {
        perror("SSL_set_fd");
        SSL_free(ssl);
        SSL_CTX_free(ssl_ctx);
        exit(EXIT_FAILURE);
    }

    // Connect to remote peer
    // If we have a broken connection, as given here, a try to connect will
    // silently abort the program with a SIGPIPE signal.

    sigpipe::CSigpipe_scoped no_signal_broken_pipe;

    if (SSL_connect(ssl) <= 0) // error, underlaying socket no connection
        perror("Error SSL_connect");

    close(sockfd);
    SSL_free(ssl);
    SSL_CTX_free(ssl_ctx);
    std::cout << "done." << std::endl;
    return EXIT_SUCCESS;
}
#endif
