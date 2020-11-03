#include <stdio.h>

#include "server.h"

int main(int argc, char** argv) {
  server_t server;
  if (argc != 4) {
    return 1;
  }

  server_run(&server, argc, argv);
  return 0;
}
