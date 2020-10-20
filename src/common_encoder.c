#include "common_encoder.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int encoder_init(encoder_t *self, const char *method, const char *key) {
  if (strcmp(method, "cesar") == 0) {
    self->codification = cesar;
  } else if (strcmp(method, "vigenere") == 0) {
    self->codification = vigenere;
  } else if ((strcmp(method, "rc4") == 0)) {
    self->codification = rc4;
  } else {
    return 1;
  }

  self->key = key;
  self->key_iterator = 0;
  strncpy((char *)self->characters_rc4, "0", 256);

  self->i_status_rc4 = 0;
  self->j_status_rc4 = 0;
  return 0;
}

void encoder_run(encoder_t *self, char *msg, unsigned char *result, int mode,
                 int readed) {
  switch (encoder_get_method(self)) {
    case cesar:
      _encoder_cesar(self, msg, result, mode, readed);
      break;
    case rc4:
      _encoder_rc4(self, msg, result);
      break;
    default:
      _encoder_vigenere(self, msg, result, mode, readed);
      break;
  }
  return;
}

void _encoder_cesar(encoder_t *self, char *msg, unsigned char *result, int mode,
                    int readed) {
  // convierto el "const char* key" en un "int" para sumar
  int key = atoi(encoder_get_key(self));
  _encoder_sum_chars(self, msg, result, (const char *)&key, 1, mode, readed);
}

void _encoder_rc4(encoder_t *self, char *msg, unsigned char *result) {
  memset(result, 0, CHUNK_SIZE_C);
  unsigned int i = self->i_status_rc4;
  unsigned int j = self->j_status_rc4;
  if (strcmp((char *)self->characters_rc4, "0") == 0) {
    _encoder_rc4_KSA(self, self->characters_rc4);
  }
  _encoder_rc4_PRGA(self, self->characters_rc4, msg, result, i, j);
}

void _encoder_rc4_KSA(encoder_t *self, unsigned char *characters) {
  const char *key = encoder_get_key(self);
  size_t key_size = strlen(key);
  for (int i_iterator = 0; i_iterator < 256; i_iterator++) {
    characters[i_iterator] = i_iterator;
  }
  int j_iterator = 0;
  for (int i_iterator = 0; i_iterator < 256; i_iterator++) {
    j_iterator =
        (j_iterator + key[i_iterator % key_size] + characters[i_iterator]) &
        255;
    _encoder_swap(characters, i_iterator, j_iterator);
  }
}

void _encoder_rc4_PRGA(encoder_t *self, unsigned char *characters, char *msg,
                       unsigned char *result, unsigned int i_iterator,
                       unsigned int j_iterator) {
  for (int msg_iterator = 0; msg_iterator < CHUNK_SIZE_C; msg_iterator++) {
    i_iterator = (i_iterator + 1) & 255;
    j_iterator = (j_iterator + characters[i_iterator]) & 255;
    _encoder_swap(characters, i_iterator, j_iterator);
    result[msg_iterator] =
        msg[msg_iterator] ^
        characters[(characters[i_iterator] + characters[j_iterator]) & 255];
  }
  self->i_status_rc4 = i_iterator;
  self->j_status_rc4 = j_iterator;
}

void _encoder_swap(unsigned char *characters, unsigned int i, unsigned int j) {
  unsigned char aux = characters[i];
  characters[i] = characters[j];
  characters[j] = aux;
}

void _encoder_vigenere(encoder_t *self, char *msg, unsigned char *result,
                       int mode, int readed) {
  const char *key = encoder_get_key(self);
  unsigned int size_key = strlen(key);
  _encoder_sum_chars(self, msg, result, key, size_key, mode, readed);
}

const char *encoder_get_key(encoder_t *self) { return self->key; }

enum CODIFICATION encoder_get_method(encoder_t *self) {
  return self->codification;
}

//"mode" indica si es para encriptar (1) o desencriptar (-1) el mensaje
void _encoder_sum_chars(encoder_t *self, char *msg, unsigned char *result,
                        const char *key, unsigned int size_key, int mode,
                        int readed) {
  for (int i = 0; i < readed; i++) {
    result[i] =
        (unsigned int)(msg[i] + (mode * key[self->key_iterator % size_key]));
    // asi se hace circular, por si se pasa de la z
    result[i] = result[i] % 256;
    self->key_iterator++;
  }
}
