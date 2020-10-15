#include <stdio.h>

#include "server.h"

int main(int argc, char** argv) {
  server_t server;
  if (argc != 4) {
    printf("Formato erroneo. Respetar: \n");
    printf("./server <server-port> --method=<method> --key=<key> \n");
    return 1;
  }
  const char* service = argv[1];
  const char* method = argv[2] + 9;
  const char* key = argv[3] + 6;
  server_run(&server, service, method, key);
  return 0;
}
