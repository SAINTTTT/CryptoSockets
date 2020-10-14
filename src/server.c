#include "server.h"
#include <stdlib.h>
#include <string.h>
#include "common_encoder.h"

#define CHUNK_SIZE 64

void server_init(server_t *self, const char *service){
    socket_init(&self->socket, 2);
    _server_connect(self, service);
}

void _server_connect(server_t *self, const char *service){
    if (socket_bind_and_listen(&self->socket, service) < 0) {
        exit(1);
    }
    if (socket_accept(&self->socket,&self->socket_peer)< 0) {
		exit(1);
	}
}


void server_run(server_t *self, const char *service, const char *method, const char *key){
    server_init(self, service);
    server_decrypt(self, method, key);
    server_finish(self);
}


void server_decrypt(server_t *self, const char *method, const char *key){
    encoder_t encoder;
    if (encoder_init(&encoder, method, key)){
        exit(1);
    }
    unsigned char encrypted_msg[CHUNK_SIZE + 1] = "\0";
    int key_iterator = 0;
    while(socket_receive(&self->socket_peer,(char*) encrypted_msg, CHUNK_SIZE)){
        encoder_decrypt(&encoder, encrypted_msg, key_iterator);
        memset(encrypted_msg,0,CHUNK_SIZE); 
    }
}


void server_finish(server_t *self){
    socket_finish(&self->socket);
    socket_finish(&self->socket_peer);
}

