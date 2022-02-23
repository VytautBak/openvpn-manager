

#include <arpa/inet.h>  //inet_addr
#include <stdbool.h>
#include <stdio.h>  //printf
#include <stdio.h>
#include <stdlib.h>
#include <string.h>      //strlen
#include <sys/socket.h>  //socket
#include <unistd.h>

#define SOCKET_ADRESS "127.0.0.1"
#define SOCKET_PORT 17111
#define MAX_LEN_RESPONSE 10000

#define NAME_LEN 25
#define ADDRESS_LEN 22
#define BYTES_LEN 20
#define TIME_LEN 20

#ifndef SOCKET_HANDLER_H
#define SOCKET_HANDLER_H

void deal_with_login_message();
int kill_client(int id);
void create_server(struct sockaddr_in *server);
int create_socket();
int connect_to_openvpn(struct sockaddr_in *server);


#endif