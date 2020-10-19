#include <getopt.h>
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
  struct option long_options[] = {
      {"method", required_argument, 0, 0},
      {"key", required_argument, 0, 0},
  };

  getopt_long(argc, argv, "service:--method:--key", long_options, &optind);
  const char* method = optarg;
  getopt_long(argc, argv, "service:--method:--key", long_options, &optind);
  const char* key = optarg;
  server_run(&server, service, method, key);
  return 0;
}
