#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <iostream>

int connect_to(const char *addr, const char *port); //Возвращает дескриптор сокета
int send_all(int s, char* buf, int *len); // Отправляет всё, что было в буфере