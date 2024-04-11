#include "cool_sockets.h"
#include <unistd.h>

class Server {
    private:
        char ip_[INET6_ADDRSTRLEN + 1];
        char port_[10];
        int sockfd_;

        int connected_sockfd_;

        struct addrinfo *servinfo_;
        void get_my_ip();
        void init_server();
        void get_acception();
    public:
        Server(char* port);
        ~Server();

};