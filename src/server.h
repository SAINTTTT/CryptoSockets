#include "common_socket.h"

typedef struct {
  tcp_socket_t socket;
  tcp_socket_t socket_peer;
} server_t;

void server_init(server_t *self, const char *service);

void _server_connect(server_t *self, const char *service);

void server_run(server_t *self, int argc, char **argv);

void server_finish(server_t *self);

void server_decrypt(server_t *self, const char *method, const char *key);
