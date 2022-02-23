#include "argp_handler.h"


static struct argp_option options[] = {
    {"socket", 's', "socket", 0, "Socket for OpenVPN management server"},
    {0}};

static struct argp argp = {options, parse_opt, "", ""};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
  char **input = state->input;
  switch (key) {
    case 's':
      *input = arg;
      break;

    default:
      return ARGP_ERR_UNKNOWN;
  }
  return 0;
}


int argp_validate(char *input, int *sock) {
  if(input == NULL){
    return -1;
  }
  char *ptr = NULL;
  *sock = strtol(input, &ptr, 10);
  if(*ptr != "") {
    fprintf(stderr, "ERROR: Socket must be a number \n");
    return -1;
  }
  if(*sock <= 0) {
    fprintf(stderr, "ERROR: Socket must be positive\n");
    return -1;
  }
  if(*sock >= 65,535) {
    fprintf(stderr, "ERROR: Socket must be in interval (0; 65535) \n");
    return -1;
  }
  return 0;
}

int get_socket(int *sock, int argc, char *argv[]) {
  char *input;
  int rc = argp_parse(&argp, argc, argv, 0, 0, &input);
  rc = argp_validate(input, sock);
  return rc;
}