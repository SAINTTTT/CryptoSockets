#ifndef CLIENT_H_
#define CLIENT_H_

#include <stdlib.h>
#include <stdio.h>
#include "common_socket.h"

typedef struct {
	tcp_socket_t socket;

} client_t;

void client_init(client_t *self, const char* host, const char* service);

void client_run(client_t *self, const char* host, const char* service, const char* method, const char* key);

void _client_connect(client_t *self, const char* host, const char* service);

int _client_read_file(FILE *self, char *buffer);

void _client_send_message(client_t *self, char *buffer);

void _client_encrypt_file(client_t *self,const char* method, const char* key);
#endif