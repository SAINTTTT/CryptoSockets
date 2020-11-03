#include "common_cesar.h"

#include <stdio.h>
#include <stdlib.h>

void cesar_init(cesar_t *self, const char *key) { self->key = atoi(key); }

void cesar_run(cesar_t *self, char *msg, unsigned char *result, int mode,
               int readed) {
  _cesar_sum_chars(self, msg, result, mode, readed);
}

void _cesar_sum_chars(cesar_t *self, char *msg, unsigned char *result, int mode,
                      int readed) {
  //
  for (int i = 0; i < readed; i++) {
    result[i] = (unsigned int)(msg[i] + (mode * self->key));
    // asi se hace circular, por si se pasa de la z
    result[i] = result[i] % 256;
  }
}
