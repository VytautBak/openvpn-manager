
#include <stdbool.h>
#include <stdio.h>  //printf
#include <stdio.h>
#include <stdlib.h>
#include <string.h>      //strlen
#include <sys/socket.h>  //socket
#include <unistd.h>
#include <libubox/blobmsg_json.h>

#ifndef CLIENT_H
#define CLIENT_H

struct client {
  char common_name[25];
  char real_address[22]; /*Longest possible address is 255.255.255.255:65535 */
  char bytes_received[20];
  char bytes_sent[20];
  char connected_since[20];
  int id;
  struct client *next; /* This is a singly-linked list */
};

int format_json_string_of_clients(char **json, struct client *client_list);
void parse_clients(char *message, struct client **client_list);
void free_client_list(struct client *client_list);



#endif