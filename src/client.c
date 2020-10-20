#include "client.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common_encoder.h"

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
    exit(EXIT_FAILURE);
  }
}

void _client_encrypt_file(client_t* self, const char* method, const char* key) {
  encoder_t encoder;
  if (encoder_init(&encoder, method, key)) {
    client_finish(self);
    exit(EXIT_FAILURE);
  }
  char buffer[CHUNK_SIZE_C + 1] = "\0";
  unsigned char encrypted_msg[CHUNK_SIZE_C + 1] = "\0";
  int read = 1;
  while (!feof(stdin) || (read != 0)) {
    read = _client_read_file(stdin, buffer);
    if (read == 0) break;
    encoder_run(&encoder, buffer, encrypted_msg, ENCRYPT, read);
    _client_send_message(self, (char*)encrypted_msg, read);
    memset(buffer, 0, CHUNK_SIZE_C);  // porque puede leer menos de 64
  }
}

int _client_read_file(FILE* file, char* buffer) {
  int bytes_read = fread(buffer, 1, CHUNK_SIZE_C, file);
  return bytes_read;
}

void _client_send_message(client_t* self, char* buffer, int readed) {
  int bytes_sent = socket_send(&self->socket, buffer, readed);
  if (bytes_sent < 0) {
    client_finish(self);
    exit(EXIT_FAILURE);
  }
}

void client_finish(client_t* self) {
  socket_shutdown(&self->socket);
  socket_finish(&self->socket);
}
