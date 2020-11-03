#ifndef __COMMON_CESAR_H__
#define __COMMON_CESAR_H__

typedef struct {
  unsigned int key;
} cesar_t;

void cesar_init(cesar_t *self, const char *key);

void cesar_run(cesar_t *self, char *msg, unsigned char *result, int mode,
               int readed);

void _cesar_sum_chars(cesar_t *self, char *msg, unsigned char *result, int mode,
                      int readed);

#endif
