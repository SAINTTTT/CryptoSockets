#include "common_vigenere.h"

#include <string.h>

void vigenere_init(vigenere_t *self, const char *key,
                   unsigned int key_iterator) {
  self->key = key;
  self->key_iterator = key_iterator;
}

void vigenere_run(vigenere_t *self, char *msg, unsigned char *result, int mode,
                  int readed) {
  _vigenere_sum_chars(self, msg, result, mode, readed);
}

void _vigenere_sum_chars(vigenere_t *self, char *msg, unsigned char *result,
                         int mode, int readed) {
  int size_key = strlen(self->key);
  for (int i = 0; i < readed; i++) {
    result[i] =
        (unsigned int)(msg[i] +
                       (mode * self->key[self->key_iterator % size_key]));
    // asi se hace circular, por si se pasa de la z
    result[i] = result[i] % 256;
    self->key_iterator++;
  }
}
