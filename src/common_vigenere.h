#ifndef __COMMON_VIGENERE_H__
#define __COMMON_VIGENERE_H__

typedef struct {
  const char *key;
  unsigned int key_iterator;
} vigenere_t;

void vigenere_init(vigenere_t *self, const char *key,
                   unsigned int key_iterator);

void vigenere_run(vigenere_t *self, char *msg, unsigned char *result, int mode,
                  int readed);

void _vigenere_sum_chars(vigenere_t *self, char *msg, unsigned char *result,
                         int mode, int readed);

#endif
