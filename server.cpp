#include "server.h"


void Server::get_my_ip() {
    int sockfd = connect_to("ipinfo.io", "80");
    int len = 33;
    char req[33] = "GET / HTTP/1.1\nHost: ipinfo.io\n\n";
    if (send_all(sockfd, req, &len) == -1) {
        printf("Can't detect ip, try again\n");
        exit(1);
    }

    char buf[1024];
    if (recv(sockfd, buf, 1024, 0) == -1) {
        exit(1);
    }

    char* found_str = strstr(buf, "ip");
    char ip[INET6_ADDRSTRLEN + 1];
    // Ставлю i + 6, потому что found_str = ip": "..."
    for (int i = 0; found_str[i + 6] != '"'; i++) {
        ip[i] = found_str[i + 6];
    }

    strcpy(ip_, ip);
}

void Server::init_server() {
    int status;
    struct addrinfo hints;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((status = getaddrinfo(NULL, port_, &hints, &servinfo_)) != 0) {
        printf("getaddrginfo error on init: %s\n", gai_strerror(status));
        exit(1);
    }
    
    sockfd_ = socket(servinfo_->ai_family, servinfo_->ai_socktype, servinfo_->ai_protocol);

    bind(sockfd_, servinfo_->ai_addr, servinfo_->ai_addrlen);

    listen(sockfd_, 1);
}

void Server::get_acception() {
    struct sockaddr_storage their_addr;
    socklen_t addr_size;

    addr_size = sizeof their_addr;
    connected_sockfd_ = accept(sockfd_, (struct sockaddr *)&their_addr, &addr_size);
    if (connected_sockfd_ == -1) {
        printf("error accepting connection\n");
        exit(1);
    }

    struct sockaddr_in *sin = (struct sockaddr_in *)&their_addr;
    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(sin->sin_addr), ip, INET_ADDRSTRLEN);


    printf("%s connected\n", ip);
}

Server::Server(char* port) {
    strcpy(port_, port);

    get_my_ip();
    printf("server configuration: \nip: %s\nport: %s\n", ip_, port_);
    init_server();

    printf("server: waiting for connection...\n");

    get_acception();
    char response[16] = "Hello, client!\n";
    int len = 16;
    send_all(connected_sockfd_, response, &len);

    while (1) {
        char buf[1024];
        memset(buf, '\0', sizeof buf);

        recv(connected_sockfd_, buf, 1024, 0);
        printf("%s", buf);
        if (buf[0] == '\r') {
            printf("Client ended session\n");
            break;
        }
        int len = strlen(buf);
        send_all(connected_sockfd_, buf, &len);
    }

    printf("Connection lost\n");
}

Server::~Server() {
    close(sockfd_);
    close(connected_sockfd_);

    freeaddrinfo(servinfo_);
}