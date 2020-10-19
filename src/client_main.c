#include <getopt.h>
#include <stdio.h>

#include "client.h"

int main(int argc, char** argv) {
  client_t client;
  if (argc != 5) {
    printf("Formato erroneo. Respetar: \n");
    printf(
        "./client <server-host> <server-port> --method=<method> --key=<key> "
        "\n");
    return 1;
  }

  const char* host = argv[1];
  const char* service = argv[2];

  struct option long_options[] = {
      {"method", required_argument, 0, 0},
      {"key", required_argument, 0, 0},
  };

  getopt_long(argc, argv, "host:service:--method:--key", long_options, &optind);
  const char* method = optarg;
  getopt_long(argc, argv, "host:service:--method:--key", long_options, &optind);
  const char* key = optarg;

  client_run(&client, host, service, method, key);

  return 0;
}
