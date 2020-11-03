#include "server.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common_encoder.h"

void server_init(server_t *self, const char *service) {
  socket_init(&self->socket);
  _server_connect(self, service);
}

void _server_connect(server_t *self, const char *service) {
  if (socket_bind_and_listen(&self->socket, service) < 0) {
    server_finish(self);
    exit(EXIT_FAILURE);
  }
  if (socket_accept(&self->socket, &self->socket_peer) < 0) {
    server_finish(self);
    exit(EXIT_FAILURE);
  }
}

void server_run(server_t *self, int argc, char **argv) {
  const char *service = argv[1];
  struct option long_options[] = {
      {"method", required_argument, 0, 0},
      {"key", required_argument, 0, 0},
  };

  getopt_long(argc, argv, "service:--method:--key", long_options, &optind);
  const char *method = optarg;
  getopt_long(argc, argv, "service:--method:--key", long_options, &optind);
  const char *key = optarg;

  server_init(self, service);
  server_decrypt(self, method, key);
  server_finish(self);
}

void server_decrypt(server_t *self, const char *method, const char *key) {
  encoder_t encoder;
  if (encoder_init(&encoder, method, key)) {
    server_finish(self);
    exit(EXIT_FAILURE);
  }
  unsigned char encrypted_msg[CHUNK_SIZE + 1] = "\0";
  unsigned char decrypted_msg[CHUNK_SIZE + 1] = "\0";
  int bytes_recv = 1;
  while (bytes_recv != 0) {  // si es 0, cerraron el socket
    bytes_recv =
        socket_receive(&self->socket_peer, (char *)encrypted_msg, CHUNK_SIZE);
    if (!bytes_recv) break;
    encoder_run(&encoder, (char *)encrypted_msg, decrypted_msg, DECRYPT,
                bytes_recv);
    fwrite(encrypted_msg, 1, bytes_recv, stdout);
    memset(encrypted_msg, 0, CHUNK_SIZE);
  }
}

void server_finish(server_t *self) {
  socket_shutdown(&self->socket_peer);
  socket_shutdown(&self->socket);
  socket_finish(&self->socket_peer);
  socket_finish(&self->socket);
}
