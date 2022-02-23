
#include "socket_handler.h"
int sock;

int init_connection_to_openvpn() {
  struct sockaddr_in server;
  create_server(&server);

  int rc = create_socket();
  if (rc != 0) return rc;

  rc = connect_to_openvpn(&server);
  if (rc != 0) return rc;
  fprintf(stdout, "Succesfully connected to OpenVPN management server!");
  deal_with_login_message();
  return 0;
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
  server->sin_port = htons(sock);
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

