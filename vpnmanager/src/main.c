#include "ubus_handler.h"
#include "argp_handler.h"
extern struct ubus_object vpnmanager_object;
extern int sock;

int main(int argc, char **argv)
{
  int rc;

	rc = get_socket(&sock, argc, argv);
	if(rc != 0) {
		return rc;
	}
	
  rc = init_connection_to_openvpn();
  if(rc != 0) {
		return rc;
	}
	
  struct ubus_context *ctx;
	uloop_init();

	ctx = ubus_connect(NULL);
	if (!ctx) {
		fprintf(stderr, "Failed to connect to ubus\n");
		return -1;
	}

	ubus_add_uloop(ctx);
	ubus_add_object(ctx, &vpnmanager_object);
	uloop_run();

	while(1) {
		printf("a");
		sleep(1);
	}

  cleanup:	
  ubus_free(ctx);
	uloop_done();
  close(sock);
	return 0;
}
