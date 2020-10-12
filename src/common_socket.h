#include <stddef.h>
#ifndef _COMMON_SOCKET_H_
#define _COMMON_SOCKET_H_

typedef struct tcp_socket {
	int fd;
	int mode; 	//1: Cliente --- 2: Servidor
} tcp_socket_t;

void socket_init(tcp_socket_t* self, int mode);

struct addrinfo* socket_getaddrinfo(const tcp_socket_t* self,
									const char* host,
									const char* service);

int socket_connect(tcp_socket_t* self, const char* host, const char* service);

int socket_bind_and_listen(tcp_socket_t* self,const char* service);

int socket_accept(tcp_socket_t* self,tcp_socket_t* socket_accept);

int socket_send(tcp_socket_t* self, const char* buffer, size_t lenght);

int socket_receive(tcp_socket_t* self, char* buffer, const size_t size);

void socket_finish(tcp_socket_t* self);

void socket_shutdown(tcp_socket_t* self);

#endif
