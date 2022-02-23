#ifndef UBUS_HANDLER_H
#define UBUS_HANDLER_H

#include <libubox/blobmsg_json.h>
#include <libubus.h>
#include "socket_handler.h"
#include <json-c/json.h>



static int vpnmanager_get(struct ubus_context *ctx, struct ubus_object *obj,
		      struct ubus_request_data *req, const char *method,
		      struct blob_attr *msg);

static int vpnmanager_kill(struct ubus_context *ctx, struct ubus_object *obj,
		      struct ubus_request_data *req, const char *method,
		      struct blob_attr *msg);

int init_ubus();


#endif