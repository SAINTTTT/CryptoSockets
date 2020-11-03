#ifndef COMMON_ENCODER_H_
#define COMMON_ENCODER_H_
#define ENCRYPT 1
#define DECRYPT -1
#define CHUNK_SIZE 64
#define CHUNK_SIZE_C 64

typedef struct {
  enum CODIFICATION { rc4, cesar, vigenere } codification;
  const char *key;
  int key_iterator;
  unsigned char characters_rc4[256];
  unsigned int i_status_rc4;
  unsigned int j_status_rc4;
} encoder_t;

int encoder_init(encoder_t *self, const char *method, const char *key);

void encoder_run(encoder_t *self, char *msg, unsigned char *encrypted, int mode,
                 int readed);

void _encoder_cesar(encoder_t *self, char *msg, unsigned char *encrypted,
                    int mode, int readed);

void _encoder_rc4(encoder_t *self, char *msg, unsigned char *encrypted);

void _encoder_vigenere(encoder_t *self, char *msg, unsigned char *encrypted,
                       int mode, int readed);

const char *encoder_get_key(encoder_t *self);

enum CODIFICATION encoder_get_method(encoder_t *self);

void encoder_update_key_iterator(encoder_t *self, unsigned int key);

void encoder_update_rc4_status(encoder_t *self, unsigned int i, unsigned int j);
#endif
