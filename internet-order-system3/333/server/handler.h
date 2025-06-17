// handler.h 服务端消息分发和业务处理头文件
#ifndef HANDLER_H
#define HANDLER_H

#include <netinet/in.h>

// 结构体：用于传递客户端socket和地址
struct client_info {
    int client_sockfd;
    struct sockaddr_in client_addr;
};

// 线程处理函数，主业务入口
void* handle_client(void* arg);

#endif // HANDLER_H