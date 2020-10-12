#ifndef COMMON_ENCODER_H_
#define COMMON_ENCODER_H_

typedef struct{
    enum CODIFICATION {rc4, cesar, vigenere} codification;
}encoder_t;

int encoder_init(encoder_t *self, const char* method, const char* key);

void encoder_encrypt(encoder_t* self, char *msg);

#endif