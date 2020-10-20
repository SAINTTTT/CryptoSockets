#ifndef COMMON_ENCODER_H_
#define COMMON_ENCODER_H_
#define ENCRYPT 1
#define DECRYPT -1

typedef struct {
  enum CODIFICATION { rc4, cesar, vigenere } codification;
  const char *key;
  unsigned char characters_rc4[256];
  unsigned int i_status_rc4;
  unsigned int j_status_rc4;
} encoder_t;

int encoder_init(encoder_t *self, const char *method, const char *key);

void encoder_run(encoder_t *self, char *msg, unsigned char *encrypted,
                 int key_iterator, int mode);

void _encoder_cesar(encoder_t *self, char *msg, unsigned char *encrypted,
                    int key_iterator, int mode);

void _encoder_rc4(encoder_t *self, char *msg, unsigned char *encrypted,
                  int key_iterator);

void _encoder_vigenere(encoder_t *self, char *msg, unsigned char *encrypted,
                       int key_iterator, int mode);

void _encoder_sum_chars(char *msg, unsigned char *encrypted, const char *key,
                        unsigned int size_key, int key_iterator, int mode);

const char *encoder_get_key(encoder_t *self);

enum CODIFICATION encoder_get_method(encoder_t *self);

void encoder_decrypt(encoder_t *encoder, char *encrypted_msg, int key_iterator,
                     unsigned char *decrypted_msg);

void _encoder_rc4_KSA(encoder_t *self, unsigned char *characters);

void _encoder_swap(unsigned char *characters, unsigned int i, unsigned int j);

void _encoder_rc4_PRGA(encoder_t *self, unsigned char *characters, char *msg,
                       unsigned char *encrypted, unsigned int i_iterator,
                       unsigned int j_iterator);

#endif
