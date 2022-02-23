#include "socket_handler.h"

extern int sock;

int main(int argc, char *argv[]) {
  struct sockaddr_in server;

  create_server(&server);

  int rc = create_socket();
  if (rc != 0) return rc;

  rc = connect_to_openvpn(&server);
  if (rc != 0) return rc;

  deal_with_login_message();
  struct client *client_list = NULL;
  rc = get_client_list(&client_list);
  //free_client_list(client_list);

  kill_client(0);

  format_table_of_clients(client_list);
  close(sock);
  return 0;
}