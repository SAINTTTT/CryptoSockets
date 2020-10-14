#ifndef COMMON_ENCODER_H_
#define COMMON_ENCODER_H_

typedef struct{
    enum CODIFICATION {rc4, cesar, vigenere} codification;
    const char* key;
}encoder_t;

int encoder_init(encoder_t *self, const char* method, const char* key);

void encoder_encrypt(encoder_t* self, char *msg, unsigned char *encrypted, int key_iterator);

void _encoder_encrypt_cesar(encoder_t* self, char *msg, unsigned char *encrypted, int key_iterator);

void _encoder_encrypt_rc4(encoder_t* self, char *msg, unsigned char *encrypted);

void _encoder_encrypt_vigenere(encoder_t* self, char *msg, unsigned char *encrypted, int key_iterator);

void _encoder_sum_chars(char* msg, unsigned char* encrypted, const char *key, unsigned int size_key, int key_iterator);

const char* encoder_get_key(encoder_t *self);

void encoder_decrypt(encoder_t *encoder, unsigned char *encrypted_msg, int key_iterator);
#endif