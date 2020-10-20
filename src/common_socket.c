#define _POSIX_C_SOURCE 200112L
#include "common_socket.h"

#include <errno.h>
#include <math.h>
#include <netdb.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define CANTIDAD_SOCKETS_LISTEN 20

void socket_init(tcp_socket_t* self, int mode) {
  self->fd = -1;
  self->mode = mode;
}

struct addrinfo* socket_getaddrinfo(const tcp_socket_t* self, const char* host,
                                    const char* service) {
  struct addrinfo hints;
  struct addrinfo* ai_list;
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_INET;       /* IPv4 (or AF_INET6 for IPv6)     */
  hints.ai_socktype = SOCK_STREAM; /* TCP  (or SOCK_DGRAM for UDP)    */
  int err = 0;
  if (self->mode == 1) {
    hints.ai_flags = 0;
    err = getaddrinfo(host, service, &hints, &ai_list);
  } else {
    hints.ai_flags = AI_PASSIVE;
    err = getaddrinfo(NULL, service, &hints, &ai_list);
  }

  if (err) printf("getaddrinfo error!\n");
  return ai_list;
}

int socket_connect(tcp_socket_t* self, const char* host, const char* service) {
  struct addrinfo* ai_list = socket_getaddrinfo(self, host, service);
  struct addrinfo* ptr;
  int err = 1;
  for (ptr = ai_list; ptr != NULL; ptr = ptr->ai_next) {
    self->fd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
    err = connect(self->fd, ptr->ai_addr, ptr->ai_addrlen);
    if (err == -1) {
      printf("Error: %s\n", strerror(errno));
    }
  }
  freeaddrinfo(ai_list);
  return err;
}

int socket_bind_and_listen(tcp_socket_t* self, const char* service) {
  struct addrinfo* ai_list = socket_getaddrinfo(self, NULL, service);
  struct addrinfo* ptr;
  int val = 1, err = 1;
  for (ptr = ai_list; ptr != NULL; ptr = ptr->ai_next) {
    self->fd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
    setsockopt(self->fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
    err = bind(self->fd, ptr->ai_addr, ptr->ai_addrlen);
    if (err == -1) {
      printf("Error: %s\n", strerror(errno));
      close(self->fd);
    }
    err = listen(self->fd, CANTIDAD_SOCKETS_LISTEN);
    if (err == -1) {
      printf("Error: %s\n", strerror(errno));
      close(self->fd);
    }
  }
  freeaddrinfo(ai_list);
  return err;
}

int socket_accept(tcp_socket_t* self, tcp_socket_t* socket_accept) {
  int fd = accept(self->fd, NULL, NULL);
  if (fd < 0) {
    printf("Error in socket accept:%s\n", strerror(errno));
  } else {
    socket_accept->fd = fd;
  }
  return fd;
}

int socket_send(tcp_socket_t* socket, const char* buffer, size_t size) {
  int bytes_sent = 0;
  while (bytes_sent < size) {
    bytes_sent =
        send(socket->fd, buffer + bytes_sent, size - bytes_sent, MSG_NOSIGNAL);
    if (bytes_sent < 0) {
      printf("Error in socket send: %s\n", strerror(errno));
      return -1;
    }
  }
  return bytes_sent;
}

int socket_receive(tcp_socket_t* socket, char* buffer, const size_t size) {
  int bytes_recv = 0;
  int connected = 1;
  while (bytes_recv < size && connected) {
    int s = recv(socket->fd, &buffer[bytes_recv], size - bytes_recv, 0);
    if (s > 0) {
      bytes_recv += s;
    } else {
      connected = 0;
      if (s < 0) printf("Error in socket receive: %s\n", strerror(errno));
    }
  }
  return bytes_recv;
}

void socket_shutdown(tcp_socket_t* self) { shutdown(self->fd, SHUT_RDWR); }

void socket_finish(tcp_socket_t* self) {
  int err = close(self->fd);
  if (err < 0) {
    printf("Error in socket close: %s\n", strerror(errno));
  }
}
