

#include <arpa/inet.h>  //inet_addr
#include <stdbool.h>
#include <stdio.h>  //printf
#include <stdio.h>
#include <stdlib.h>
#include <string.h>      //strlen
#include <sys/socket.h>  //socket
#include <unistd.h>
#include <libubox/blobmsg_json.h>


#define SOCKET_ADRESS "127.0.0.1"
#define MAX_LEN_RESPONSE 100000

#ifndef SOCKET_HANDLER_H
#define SOCKET_HANDLER_H


int init_connection_to_openvpn();
void deal_with_login_message();
int kill_client(int id);
void create_server(struct sockaddr_in *server);
int create_socket();
int connect_to_openvpn(struct sockaddr_in *server);
#endif