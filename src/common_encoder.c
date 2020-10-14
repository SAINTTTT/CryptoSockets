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

void encoder_encrypt(encoder_t* self, char *msg, unsigned char *encrypted, int key_iterator){
    switch (self->codification){
    case cesar:
        _encoder_encrypt_cesar(self,msg, encrypted,key_iterator);
        break;
    case rc4:
        _encoder_encrypt_rc4(self,msg,encrypted);
        break;
    default:
        _encoder_encrypt_vigenere(self,msg,encrypted,key_iterator);
        break;
    }

    return;    
}


void _encoder_encrypt_cesar(encoder_t* self, char *msg, unsigned char *encrypted, int key_iterator){
    int key = atoi(encoder_get_key(self));              //convierto el "const char* key" en un "int" para sumar
    _encoder_sum_chars(msg,encrypted,(const char *) &key, 1, key_iterator);

}

void _encoder_encrypt_rc4(encoder_t* self, char *msg, unsigned char *encrypted){
    printf("rc4");

}

void _encoder_encrypt_vigenere(encoder_t* self, char *msg, unsigned char *encrypted, int key_iterator){
    const char *key = encoder_get_key(self);
    unsigned int size_key = strlen(key);
    _encoder_sum_chars(msg, encrypted, key, size_key, key_iterator);
}

const char* encoder_get_key(encoder_t *self) { 
    return self->key;
}
   
   
void _encoder_sum_chars(char* msg, unsigned char* encrypted, const char *key, unsigned int size_key, int key_iterator){
    for (int i = 0; i < CHUNK_SIZE; i++) {
        encrypted[i] = (unsigned int)( msg[i] + key[key_iterator % size_key]);
        encrypted[i] = encrypted[i] % 256;          //asi se hace circular, por si se pasa de la z
        key_iterator++;
    }
}