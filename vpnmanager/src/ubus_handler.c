
#include "ubus_handler.h"

/*
 *The enumaration array is used to specifie how much arguments will our
 * methods accepted. Also to say trough which index which argument will
 * be reacheble.
 *
 *  */

enum {
  VPNMANAGER_VALUE,
  __VPNMANAGER_MAX
};

/*
 * This policy structure is used to determine the type of the arguments
 * that can be passed to some kind of method.
 * This structure will be used in another structure applying this policy
 * to our selected method.
 * */

static const struct blobmsg_policy kill_policy[] = {
    [VPNMANAGER_VALUE] = {.name = "CID", .type = BLOBMSG_TYPE_INT32},
};

/*
 * This structure is used to register available methods.
 * If a method accepts arguments, the method should have a policy.
 * */

static const struct ubus_method vpnmanager_methods[] = {
    UBUS_METHOD_NOARG("get_clients", vpnmanager_get),
    UBUS_METHOD("kill", vpnmanager_kill, kill_policy)};

/*
 * This structure is used to define the type of our object with methods.
 * */

static struct ubus_object_type vpnmanager_object_type =
    UBUS_OBJECT_TYPE("vpnmanager", vpnmanager_methods);

/*
 * This structure is used to register our program as an ubus object
 * with our methods and other neccessary data.
 * */

struct ubus_object vpnmanager_object = {
    .name = "vpnmanager",
    .type = &vpnmanager_object_type,
    .methods = vpnmanager_methods,
    .n_methods = ARRAY_SIZE(vpnmanager_methods),
};

/*
 * This method is used as a callback function
 * to return the value of our variable count.
 * All the arguments ar neccessary.
 * Using blobmsg object, our variable is packed ant returned
 * through ubus server.
 * */
static int vpnmanager_get(struct ubus_context *ctx, struct ubus_object *obj,
                          struct ubus_request_data *req, const char *method,
                          struct blob_attr *msg) {
  struct blob_buf b = {};

  blob_buf_init(&b, 0);

  struct client *client_list = NULL;
  int rc = get_client_list(&client_list);
  if (rc != 0) return -1;

  char *json = NULL;
  rc = format_json_string_of_clients(&json, client_list);

  if (rc != 0) {
    blobmsg_add_string(&b, "response", "No clients connected");
  } else {
    blobmsg_add_json_element(&b, "", json_tokener_parse(json));
  }

  ubus_send_reply(ctx, req, b.head);
  blob_buf_free(&b);
	free(json);
  free_client_list(client_list);
  return 0;
}

/*
 * This method is used to read the argument value which is passed over ubus
 * and append that value to our global variable.
 * All the arguments are neccessary.
 * */
static int vpnmanager_kill(struct ubus_context *ctx, struct ubus_object *obj,
                           struct ubus_request_data *req, const char *method,
                           struct blob_attr *msg) {
  /*
   * This structure is used to store the arguments which are passed
   * through ubus.
   * __COUNTER_MAX in this scenario is equal to 1.
   * So this structure will hold only one variable.
   * */
  struct blob_attr *tb[__VPNMANAGER_MAX];
  struct blob_buf b = {};

  blobmsg_parse(kill_policy, __VPNMANAGER_MAX, tb, blob_data(msg), blob_len(msg));

  if (!tb[VPNMANAGER_VALUE])
    return UBUS_STATUS_INVALID_ARGUMENT;

  /*
   * This is the place where the value is extracted and appended to our
   * variable.
   * COUNTER_VALUE in this scenario is equal to 0. 0 indicates the first
   * array element.
   * blogmsg_get_u32 parses the value which is appended to the variable.
   * */

  blob_buf_init(&b, 0);

  int cid = blobmsg_get_u32(tb[VPNMANAGER_VALUE]);
  int rc = kill_client(cid);
  if (rc == 0) {
    blobmsg_add_string(&b, "response", "Success");
  } else {
    blobmsg_add_string(&b, "response", "Failed");
  }

  ubus_send_reply(ctx, req, b.head);
  blob_buf_free(&b);

  return 0;
}
