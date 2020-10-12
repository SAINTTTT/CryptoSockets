#include "common_encoder.h"
#include <stdio.h>

int encoder_init(encoder_t *self, const char* method, const char* key){
    return 0;
}

void encoder_encrypt(encoder_t* self, char *msg){
    printf("encoder: %s", msg);
    return;    
}