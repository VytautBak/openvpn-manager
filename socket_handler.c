
#include "socket_handler.h"

struct client {
  char common_name[NAME_LEN];
  char real_address[ADDRESS_LEN]; /*Longest possible address is 255.255.255.255:65535 */
  char bytes_received[BYTES_LEN];
  char bytes_sent[BYTES_LEN];
  char connected_since[TIME_LEN];
  int id;
  struct client *next; /* This is a singly-linked list */
};

int sock;

void format_table_of_clients(struct client *client_list) {
  struct client *curr = client_list;
  int n = 0;
  while(curr != NULL) {
    curr = curr->next;
    n++;
  }

  int size = sizeof(char) * (n  + 1) * 100;
  char *table = malloc(size);
  memset(table, 0, size);
  int j = sprintf(table, "%-5s\t %-25s\t %-20s\t %-20s\t %-20s\t %-20s\t\n", "ID", "Name", "Real address", "Bytes sent", "Bytes received", "Conn. since");
    curr = client_list;
  for(int i = 0; i < n; i++) {
    char id[6];
    sprintf(id, "%d", curr->id);
    j += sprintf(table + j, "%-5s\t %-25s\t %-20s\t %-20s\t %-20s\t %-20s\t\n", id, curr->common_name, curr->real_address, curr->bytes_sent, curr->bytes_received, curr->connected_since);
    curr = curr->next;
  }
}


void deal_with_login_message() {
  char response[200];
  int rc = recv(sock, response, 200, 0);
}

int kill_client(int id) {
  char message[50];
  char response[MAX_LEN_RESPONSE];
  sprintf(message, "client-kill %d\n", id);

  int rc = send(sock, message, strlen(message), 0);
  if (rc != strlen(message)) {
    fprintf(stderr, "ERROR: Failed to send message to OpenVPN management server\n");
    return -1;
  }
  sleep(1);

  bool success = false;
  for (int i = 0; i < 3; i++) {
    fprintf(stdout, "INFO: Attempting to parse response from OpenVPN management server n = %d\n", i + 1);
    rc = recv(sock, response, MAX_LEN_RESPONSE, 0);
    if (rc > 0) {
      success = true;
      fprintf(stdout, "INFO: Succesfully parsed response from OpenVPN management server\n");
      break;
    }
  }
  if (success == false) {
    fprintf(stderr, "ERROR: Did not receive message from OpenVPN management server \n");
    return -1;
  }
  char *ptr = strtok(response, ",\n\0");


  while(ptr != NULL) {
    printf(ptr);
    if(strcmp(ptr, "SUCCESS: client-kill command succeeded\r") == 0) {
      fprintf(stdout, "INFO: Succesfully killed client id = %d\n", id);
      return 0;
    }
    else if(strcmp(ptr, "ERROR: client-kill command failed\r") == 0) {
      fprintf(stderr, "ERROR: Failed to kill client id = %d\n", id);
      return -1;
    }
    ptr = strtok(NULL, "\n\0");
  }

  fprintf(stderr, "ERROR: Could not determine if client id = %d has been killed\n");
  return -1;
}

void create_server(struct sockaddr_in *server) {
  server->sin_addr.s_addr = inet_addr(SOCKET_ADRESS);
  server->sin_family = AF_INET;
  server->sin_port = htons(SOCKET_PORT);
}

int create_socket() {
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock == -1) {
    fprintf(stderr, "ERROR: Could not create socket");
    return -1;
  }
  return 0;
}

int get_client_list(struct client **client_list) {
  char response[MAX_LEN_RESPONSE];

  char *tmp = "status 2\n";
  int rc = send(sock, tmp, strlen(tmp), 0);
  if (rc != strlen(tmp)) {
    fprintf(stderr, "ERROR: Failed to send message to OpenVPN management server\n");
    return -1;
  }

  /* After sending a message, it is best to wait a second before attempting to parse the response*/
  sleep(1);

  bool success = false;
  for (int i = 0; i < 3; i++) {
    fprintf(stdout, "INFO: Attempting to parse response from OpenVPN management server n = %d\n", i + 1);
    rc = recv(sock, response, MAX_LEN_RESPONSE, 0);
    if (rc > 0) {
      success = true;
      break;
    }
    sleep(1);
  }

  if (success == false) {
    fprintf(stderr, "ERROR: Did not receive message from OpenVPN management server \n");
    return -1;
  }

  parse_clients(response, client_list);
  return 0;
}

int connect_to_openvpn(struct sockaddr_in *server) {
  int rc = connect(sock, (struct sockaddr *)server, sizeof(*server));
  if (rc < 0) {
    fprintf(stderr, "ERROR: Failed connecting to server");
    return -1;
  }
  return 0;
}

void parse_clients(char *message, struct client **client_list) {
  char *ptr;
  ptr = strtok(message, ",\n");

  /* Loop through message to find first 'CLIENT_LIST' and then ignore it
    (because it is used in header before the list of clients)	*/

  while (strcmp(ptr, "CLIENT_LIST") != 0) {
    ptr = strtok(NULL, ",\n");
  }
  ptr = strtok(NULL, ",\n");

  while (ptr != NULL) {
    if (strcmp(ptr, "CLIENT_LIST") == 0) {
      struct client *curr = malloc(sizeof(struct client));
      curr->next = NULL;
      ptr = strtok(NULL, ",\n");
      strcpy(curr->common_name, ptr);
      ptr = strtok(NULL, ",\n");
      strcpy(curr->real_address, ptr);
      ptr = strtok(NULL, ",\n");
      ptr = strtok(NULL, ",\n");
      strcpy(curr->bytes_received, ptr);
      ptr = strtok(NULL, ",\n");
      strcpy(curr->bytes_sent, ptr);

      ptr = strtok(NULL, ",\n");
      strcpy(curr->connected_since, ptr);
      ptr = strtok(NULL, ",\n");
      ptr = strtok(NULL, ",\n");
      ptr = strtok(NULL, ",\n");
      curr->id = strtol(ptr, NULL, 10);
      if (*client_list == NULL) {
        *client_list = curr;
      } else {
        curr->next = *client_list;
        *client_list = curr;
      }
    }
    ptr = strtok(NULL, ",\n");
  }
}

void free_client_list(struct client *client_list) {
  struct client *curr = client_list;
  struct client *tmp;
  while (curr != NULL) {
    tmp = curr;
    curr = curr->next;
    free(tmp);
  }
}