#include "client.h"

#include <stdio.h>
#include <string.h>

#include "common_encoder.h"
#define CHUNK_SIZE 64

// Devuelve el cliente conectado, listo para enviar bytes
void client_init(client_t* self, const char* host, const char* service) {
  socket_init(&self->socket, 1);
  _client_connect(self, host, service);
}

void client_run(client_t* self, const char* host, const char* service,
                const char* method, const char* key) {
  client_init(self, host, service);
  _client_encrypt_file(self, method, key);
  client_finish(self);
}

void _client_connect(client_t* self, const char* host, const char* service) {
  int err = socket_connect(&self->socket, host, service);
  if (err < 0) {
    client_finish(self);
    exit(1);
  }
}

void _client_encrypt_file(client_t* self, const char* method, const char* key) {
  encoder_t encoder;
  if (encoder_init(&encoder, method, key)) {
    client_finish(self);
    exit(1);
  }
  FILE* fp = stdin;
  char buffer[CHUNK_SIZE + 1] = "\0";
  unsigned char encrypted_msg[CHUNK_SIZE + 1] = "\0";
  int read = 1;
  int key_iterator = 0;
  while (read) {
    read = _client_read_file(fp, buffer);
    if (!read) break;
    encoder_encrypt(&encoder, buffer, encrypted_msg, key_iterator);
    _client_send_message(self, (char*)encrypted_msg);
    memset(buffer, 0,
           CHUNK_SIZE);  // porque puede leer menos de 64 y quedar con basura
  }
}

int _client_read_file(FILE* self, char* buffer) {
  int bytes_read = fread(buffer, 1, CHUNK_SIZE, self);
  return bytes_read;
}

void _client_send_message(client_t* self, char* buffer) {
  int bytes_sent = socket_send(&self->socket, buffer, CHUNK_SIZE + 1);
  if (bytes_sent < 0) {
    client_finish(self);
    exit(EXIT_FAILURE);
  }
}

void client_finish(client_t* self) {
  socket_shutdown(&self->socket);
  socket_finish(&self->socket);
}
