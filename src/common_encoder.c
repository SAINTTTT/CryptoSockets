#include "common_encoder.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CHUNK_SIZE 64

int encoder_init(encoder_t *self, const char* method, const char* key){
    if (strcmp(method,"cesar") == 0){
        self->codification = cesar;
    } else if (strcmp(method,"vigenere") == 0){
        self->codification = vigenere;
    } else if ((strcmp(method,"rc4") == 0)) {
        self->codification = rc4;
    } else {
        printf("Modo no soportado por el codificador\n");
        printf("Elegir entre {cesar, vigenere, rc4}\n");
        return 1;
    }
    
    self->key = key;
    return 0;
}

void encoder_encrypt(encoder_t* self, char *msg, char *encrypted){
    switch (self->codification){
    case cesar:
        _encoder_encrypt_cesar(self,msg, encrypted);
        break;
    case rc4:
        _encoder_encrypt_rc4(self,msg,encrypted);
        break;
    default:
        _encoder_encrypt_vigenere(self,msg,encrypted);
        break;
    }

    return;    
}


void _encoder_encrypt_cesar(encoder_t* self, char *msg, char *encrypted){
    int key = atoi(encoder_get_key(self));              //convierto el "const char* key" en un "int" para sumar
    for (size_t i = 0; i < CHUNK_SIZE; i++) {
        encrypted[i] = (unsigned int) msg[i] + key;
        encrypted[i] = encrypted[i] % 256;
    }

}

void _encoder_encrypt_rc4(encoder_t* self, char *msg,char *encrypted){
    

}

void _encoder_encrypt_vigenere(encoder_t* self, char *msg, char *encrypted){
    

}

const char* encoder_get_key(encoder_t *self) { 
    return self->key;
}