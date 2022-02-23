#include <argp.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef ARGP_HANDLER
#define ARGP_HANDLER
int get_socket(int *sock, int argc, char *argv[]);
int argp_validate(char *input, int *sock);
static error_t parse_opt(int key, char *arg, struct argp_state *state);


#endif