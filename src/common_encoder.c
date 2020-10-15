#include "common_encoder.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHUNK_SIZE 64
#define ENCRYPT 1
#define DECRYPT -1

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
  return 0;
}

void encoder_encrypt(encoder_t *self, char *msg, unsigned char *encrypted,
                     int key_iterator) {
  switch (encoder_get_method(self)) {
    case cesar:
      _encoder_cesar(self, msg, encrypted, key_iterator, ENCRYPT);
      break;
    case rc4:
      _encoder_rc4(self, msg, encrypted, key_iterator, ENCRYPT);
      break;
    default:
      _encoder_vigenere(self, msg, encrypted, key_iterator, ENCRYPT);
      break;
  }

  return;
}

void _encoder_cesar(encoder_t *self, char *msg, unsigned char *encrypted,
                    int key_iterator, int mode) {
  // convierto el "const char* key" en un "int" para sumar
  int key = atoi(encoder_get_key(self));
  _encoder_sum_chars(msg, encrypted, (const char *)&key, 1, key_iterator, mode);
}

void _encoder_rc4(encoder_t *self, char *msg, unsigned char *encrypted,
                  int key_iterator, int mode) {
  unsigned char characters[256] = "0";
  unsigned int i = 0;
  unsigned int j = 0;
  _encoder_rc4_KSA(self, characters, i, j, key_iterator);
  _encoder_rc4_PRGA(self, characters, msg, encrypted, i, j);
}

void _encoder_vigenere(encoder_t *self, char *msg, unsigned char *encrypted,
                       int key_iterator, int mode) {
  const char *key = encoder_get_key(self);
  unsigned int size_key = strlen(key);
  _encoder_sum_chars(msg, encrypted, key, size_key, key_iterator, mode);
}

const char *encoder_get_key(encoder_t *self) { return self->key; }

enum CODIFICATION encoder_get_method(encoder_t *self) {
  return self->codification;
}

//"mode" indica si es para encriptar (1) o desencriptar (-1) el mensaje
void _encoder_sum_chars(char *msg, unsigned char *encrypted, const char *key,
                        unsigned int size_key, int key_iterator, int mode) {
  for (int i = 0; i < CHUNK_SIZE; i++) {
    encrypted[i] =
        (unsigned int)(msg[i] + (mode * key[key_iterator % size_key]));
    // asi se hace circular, por si se pasa de la z
    encrypted[i] = encrypted[i] % 256;
    key_iterator++;
  }
}

void encoder_decrypt(encoder_t *self, char *encrypted_msg, int key_iterator,
                     unsigned char *decrypted_msg) {
  switch (encoder_get_method(self)) {
    case cesar:
      _encoder_cesar(self, encrypted_msg, decrypted_msg, key_iterator, DECRYPT);
      break;
    case rc4:
      _encoder_rc4(self, encrypted_msg, decrypted_msg, key_iterator, DECRYPT);
      break;
    default:
      _encoder_vigenere(self, encrypted_msg, decrypted_msg, key_iterator,
                        DECRYPT);
      break;
  }
  printf("%s", decrypted_msg);
  return;
}

void _encoder_rc4_KSA(encoder_t *self, unsigned char *characters,
                      unsigned int i_iterator, unsigned int j_iterator,
                      int key_iterator) {
  const char *key = encoder_get_key(self);
  size_t key_size = strlen(key);
  for (i_iterator = 0; i_iterator < 256; i_iterator++) {
    characters[i_iterator] = i_iterator;
  }

  for (i_iterator = j_iterator = 0; i_iterator < 256; i_iterator++) {
    j_iterator =
        (j_iterator + key[key_iterator % key_size] + characters[i_iterator]) &
        255;
    _encoder_swap(characters, i_iterator, j_iterator);
    key_iterator++;
  }
}

void _encoder_swap(unsigned char *characters, unsigned int i, unsigned int j) {
  unsigned char aux = characters[i];
  characters[i] = characters[j];
  characters[j] = aux;
}

void _encoder_rc4_PRGA(encoder_t *self, unsigned char *characters, char *msg,
                       unsigned char *encrypted, unsigned int i_iterator,
                       unsigned int j_iterator) {
  for (int msg_iterator = 0; msg_iterator < strlen(msg); msg_iterator++) {
    i_iterator = (i_iterator + 1) & 255;
    j_iterator = (j_iterator + characters[i_iterator]) & 255;
    _encoder_swap(characters, i_iterator, j_iterator);
    encrypted[msg_iterator] =
        msg[msg_iterator] ^
        characters[(characters[i_iterator] + characters[j_iterator]) & 255];
  }
}
