#include "server.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common_encoder.h"

#define CHUNK_SIZE 64

void server_init(server_t *self, const char *service) {
  socket_init(&self->socket, 2);
  _server_connect(self, service);
}

void _server_connect(server_t *self, const char *service) {
  if (socket_bind_and_listen(&self->socket, service) < 0) {
    exit(1);
  }
  if (socket_accept(&self->socket, &self->socket_peer) < 0) {
    exit(1);
  }
}

void server_run(server_t *self, const char *service, const char *method,
                const char *key) {
  server_init(self, service);
  server_decrypt(self, method, key);
  server_finish(self);
}

void server_decrypt(server_t *self, const char *method, const char *key) {
  encoder_t encoder;
  if (encoder_init(&encoder, method, key)) {
    exit(1);
  }
  unsigned char encrypted_msg[CHUNK_SIZE + 1] = "\0";
  unsigned char decrypted_msg[CHUNK_SIZE + 1] = "\0";
  int key_iterator = 0;
  int bytes_recv = 1;
  while (bytes_recv != 0) {  // si es 0, cerraron el socket
    bytes_recv = socket_receive(&self->socket_peer, (char *)encrypted_msg,
                                CHUNK_SIZE + 1);
    encoder_decrypt(&encoder, (char *)encrypted_msg, key_iterator,
                    decrypted_msg);
    // printf("%s", decrypted_msg);
    memset(encrypted_msg, 0, CHUNK_SIZE);
  }
}

void server_finish(server_t *self) {
  socket_shutdown(&self->socket);
  socket_shutdown(&self->socket_peer);
  socket_finish(&self->socket);
  socket_finish(&self->socket_peer);
}
