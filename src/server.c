#include "server.h"
#include <stdlib.h>

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

}

void server_finish(server_t *self){
    socket_finish(&self->socket);
    socket_finish(&self->socket_peer);
}