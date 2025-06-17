#ifndef HANDLER_H
#define HANDLER_H

#include <netinet/in.h>

struct client_info {
    int client_sockfd;
    struct sockaddr_in client_addr;
};

void* handle_client(void* arg);

#endif