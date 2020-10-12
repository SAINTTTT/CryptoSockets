#include <stdio.h>
#include "client.h"

int main(int argc, char** argv){
    client_t client;
    if (argc != 5){
        printf("Formato erroneo. Respetar: \n");
        printf("./client <server-host> <server-port> --method=<method> --key=<key> \n");
        return 1;
    }
    const char* host = argv[1];
    const char* service = argv[2];
    const char* method = argv[3]; 
    const char* key = argv[4];
    client_run(&client, host, service, method, key);

    return 0;
}