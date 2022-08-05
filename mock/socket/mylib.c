// simple example to handle sockets
// Author: 2021-03-09 Ingo Höft <Ingo@Hoeft-online.de>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>


static int close_socket(int pSock) {
    //int close(int fd);
    if (close(pSock) == -1) {;
        fprintf(stderr, "mysocket: error closing socket\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int mysocket()
{
    struct sockaddr_in my_addr, peer_addr;

    memset(&my_addr, 0, sizeof(my_addr));

    // get socket file descriptor
    //int socket(int domain, int type, int protocol);
	int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        fprintf(stderr, "mysocket: error getting socket\n");
        return EXIT_FAILURE;
    }

    // fill address structure
    my_addr.sin_family = AF_INET;
    if (inet_pton(AF_INET, "192.168.24.130", &my_addr.sin_addr.s_addr) != 1) {
        fprintf(stderr, "mysocket: error inet_pton\n");
        close_socket(sock);
        return EXIT_FAILURE;
    }

    // bind address to the socket
    //int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
    if (bind(sock, (struct sockaddr*)&my_addr, sizeof(my_addr)) == -1) {
        fprintf(stderr, "mysocket: error bind to socket\n");
        close_socket(sock);
        return EXIT_FAILURE;
    }

    // listen on the socket
    //listen_error = listen(s->fd, SOMAXCONN);
    if (listen(sock, 15) == -1) {
        fprintf(stderr, "mysocket: error listen on socket\n");
        close_socket(sock);
        return EXIT_FAILURE;
    }

    // accept connections from a peer
    //int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
    socklen_t peer_addr_size = sizeof(peer_addr);
    printf("mysocket:listening on socket and accepting...\nfinish with ^C\n");
    int cfd = accept(sock, (struct sockaddr *) &peer_addr, &peer_addr_size);
    if (cfd == -1) {
        fprintf(stderr, "mysocket: error accepting on socket\n");
        close_socket(sock);
        return EXIT_FAILURE;
    }

    return close_socket(sock);
}

#if defined (EXECUTABLE)
int main(int argc, char **argv)
{
    return mysocket();
}
#endif
