#ifndef CLIENT_H_
#define CLIENT_H_

#include <stdio.h>
#include <stdlib.h>

#include "common_socket.h"

typedef struct {
  tcp_socket_t socket;
} client_t;

void client_init(client_t* self, char* host, char* service);

void client_run(client_t* self, int argc, char** argv);

void _client_connect(client_t* self, const char* host, const char* service);

int _client_read_file(FILE* self, char* buffer);

void _client_send_message(client_t* self, char* buffer, int readed);

void _client_encrypt_file(client_t* self, const char* method, const char* key);

void client_finish(client_t* self);
#endif
