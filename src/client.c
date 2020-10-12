#include "client.h"
#include "common_encoder.h"
#include <stdio.h>
#include <string.h>
#define CHUNK_SIZE 64

//Devuelve el cliente conectado, listo para enviar bytes
void client_init(client_t *self, const char* host, const char* service){
    socket_init(&self->socket,1);
    _client_connect(self, host, service);
}


void client_run(client_t *self, const char* host, const char* service, const char* method, const char* key){
    client_init(self, host, service);
    _client_encrypt_file(self, method, key);

}


void _client_connect(client_t *self, const char* host, const char* service){
    int err = socket_connect(&self->socket, host, service);
    if (err < 0){
        //manejo de errores
        exit(1);
    }
}

    
void _client_encrypt_file(client_t *self,const char* method, const char* key){
    encoder_t encoder;
    if (encoder_init(&encoder, method, key)){
        printf("Modo no soportado por el codificador\n");
        printf("Elegir entre {cesar, vigenere, rc4}\n");
        exit(1);
    }
    FILE *fp = stdin;
    char buffer[CHUNK_SIZE + 1] = "\0";
    int read = 1;
    while (read){
        read = _client_read_file(fp, buffer);
        encoder_encrypt(&encoder, buffer);
        _client_send_message(self, buffer);
        memset(buffer,0,CHUNK_SIZE);
    }
}


int _client_read_file(FILE *self, char *buffer){
    int bytes_read = fread(buffer,CHUNK_SIZE,1,self);
    return bytes_read;
}

void _client_send_message(client_t *self, char *buffer){
    int bytes_sent = socket_send(&self->socket, buffer, CHUNK_SIZE+1);
    if (bytes_sent < 0) exit(EXIT_FAILURE);
}
