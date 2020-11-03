#include "common_rc4.h"

#include <stdio.h>
#include <string.h>
#define CHUNK_SIZE 64

void rc4_init(rc4_t* self, unsigned char* key, unsigned int i, unsigned int j) {
  self->key = (char*)key;
  self->i_iterator = i;
  self->j_iterator = j;
  strncpy((char*)self->characters, "0", 256);
}

void rc4_KSA(rc4_t* self) {
  size_t key_size = strlen(self->key);
  for (int i_iterator = 0; i_iterator < 256; i_iterator++) {
    self->characters[i_iterator] = i_iterator;
  }
  int j_iterator = 0;
  for (int i_iterator = 0; i_iterator < 256; i_iterator++) {
    j_iterator = (j_iterator + self->key[i_iterator % key_size] +
                  self->characters[i_iterator]) &
                 255;
    _rc4_swap(self->characters, i_iterator, j_iterator);
  }
}

void rc4_init_characters(rc4_t* self, unsigned char* characters) {
  memcpy((char*)self->characters, (char*)characters, 256);
}

void rc4_PRGA(rc4_t* self, char* msg, unsigned char* result) {
  for (int msg_iterator = 0; msg_iterator < CHUNK_SIZE; msg_iterator++) {
    self->i_iterator = (self->i_iterator + 1) & 255;
    self->j_iterator =
        (self->j_iterator + self->characters[self->i_iterator]) & 255;
    _rc4_swap(self->characters, self->i_iterator, self->j_iterator);
    result[msg_iterator] =
        msg[msg_iterator] ^
        self->characters[(self->characters[self->i_iterator] +
                          self->characters[self->j_iterator]) &
                         255];
  }
}

void _rc4_swap(unsigned char* characters, unsigned int i, unsigned int j) {
  unsigned char aux = characters[i];
  characters[i] = characters[j];
  characters[j] = aux;
}
