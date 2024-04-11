#include "server.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Wrong arguments\n");
        exit(1);
    }

    Server serv(argv[1]);
}