#include "common_encoder.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common_cesar.h"
#include "common_rc4.h"
#include "common_vigenere.h"

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
  cesar_t encoder_cesar;
  cesar_init(&encoder_cesar, self->key);
  cesar_run(&encoder_cesar, msg, result, mode, readed);
}

void _encoder_vigenere(encoder_t *self, char *msg, unsigned char *result,
                       int mode, int readed) {
  vigenere_t encoder_vigenere;
  vigenere_init(&encoder_vigenere, self->key, self->key_iterator);
  vigenere_run(&encoder_vigenere, msg, result, mode, readed);
  encoder_update_key_iterator(self, encoder_vigenere.key_iterator);
}

void _encoder_rc4(encoder_t *self, char *msg, unsigned char *result) {
  memset(result, 0, CHUNK_SIZE_C);
  rc4_t rc4;
  rc4_init(&rc4, (unsigned char *)self->key, self->i_status_rc4,
           self->j_status_rc4);
  if (strcmp((char *)self->characters_rc4, "0") == 0) {
    rc4_KSA(&rc4);
    memcpy((char *)self->characters_rc4, (char *)rc4.characters, 255);
  } else {
    rc4_init_characters(&rc4, self->characters_rc4);
  }
  rc4_PRGA(&rc4, msg, result);
  encoder_update_rc4_status(self, rc4.i_iterator, rc4.j_iterator);
}

enum CODIFICATION encoder_get_method(encoder_t *self) {
  return self->codification;
}

void encoder_update_key_iterator(encoder_t *self, unsigned int key) {
  self->key_iterator = key;
}

void encoder_update_rc4_status(encoder_t *self, unsigned int i,
                               unsigned int j) {
  self->i_status_rc4 = i;
  self->j_status_rc4 = j;
}
