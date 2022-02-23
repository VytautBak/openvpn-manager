#include "client.h"


int format_json_string_of_clients(char **json, struct client *client_list) {
  /* We need to calculate how many clients are there so we can allocate
  a long enough string */ 
  struct client *curr = client_list;
  int n = 0;
  while(curr != NULL) {
    curr = curr->next;
    n++;
  }

  if(n == 0) {
    return -1;
  }


  size_t size = 200 * n * sizeof(char);
  *json = malloc(size);
  int j = sprintf(*json, "[");

  curr = client_list;

  for(int i = 0; i < n; i++) {
  j += sprintf(*json +j, "{"
	"\"CID\": %d,"
	"\"Name\": \"%s\","
	"\"Real address\": \"%s\","
	"\"Bytes sent\": %s,"
	"\"Bytes received\": %s,"
	"\"Conn. since\": \"%s\""
                          "},", curr->id, curr->common_name, curr->real_address, curr->bytes_sent, curr->bytes_received, curr->connected_since);
    curr = curr->next;
  }
  /* Replace the last ',' with ']' */
  j += sprintf(*json + j - 1, "]");
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