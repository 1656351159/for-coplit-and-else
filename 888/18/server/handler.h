#ifndef HANDLER_H
#define HANDLER_H

#include <netinet/in.h>
#include "../common/protocol.h"

// 客户端信息结构体，便于线程传参
struct client_info {
    int client_sockfd;
    struct sockaddr_in client_addr;
};

// 线程处理函数声明
void* handle_client(void* arg);

#endif // HANDLER_H