#include "cool_sockets.h"

int connect_to(const char *addr, const char *port) {
    int status;
    struct addrinfo hints;
    struct addrinfo *res;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((status = getaddrinfo(addr, port, &hints, &res)) != 0) {
        printf("getaddrinfo error on connection to %s %s: %s'\n", addr, port, gai_strerror(status));
        exit(1);
    }

    int sockfd;

    bool connected = false;

    for (struct addrinfo *p = res; p != nullptr; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) != -1) {
            if (connect(sockfd, p->ai_addr, p->ai_addrlen) != -1) {
                connected = true;
                break;
            }
        }
    }

    if (connected) {
        printf("succesfully connected to %s %s\n", addr, port);
    } else {
        printf("can't connect to %s %s\n", addr, port);
        exit(1);
    }

    freeaddrinfo(res);

    return sockfd;
}

int send_all(int s, char* buf, int *len) {
    int total = 0;
    int bytesleft = *len;
    int n;

    while (total < *len) {
        n = send(s, buf + total, bytesleft, 0);
        if (n == -1) {
            break;
        }
        total += n;
        bytesleft -= n;
    }

    *len = total;

    return (n == -1 ? -1 : 0);
}