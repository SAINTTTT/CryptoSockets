#include "client.h"

int main(int argc, char** argv) {
  client_t client;
  if (argc != 5) {
    return 1;
  }

  client_run(&client, argc, argv);

  return 0;
}
