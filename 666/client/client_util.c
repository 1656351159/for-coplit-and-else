// 客户端通用工具实现（如输入处理、socket包装等）

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include "client_util.h"

// 输入字符串（带提示，去除换行符）
void input_str(const char* prompt, char* buf, int size) {
    printf("%s", prompt);
    if (fgets(buf, size, stdin)) {
        buf[strcspn(buf, "\n")] = 0;
    }
}

// 发送数据（简单包装，可扩展成带协议的发送）
int send_data(int sockfd, const void* buf, size_t len) {
    ssize_t sent = send(sockfd, buf, len, 0);
    if (sent == -1) {
        perror("send error");
        return -1;
    }
    return (int)sent;
}

// 接收数据（简单包装）
int recv_data(int sockfd, void* buf, size_t maxlen) {
    ssize_t recved = recv(sockfd, buf, maxlen - 1, 0);
    if (recved < 0) {
        perror("recv error");
        return -1;
    }
    ((char*)buf)[recved] = '\0'; // 确保字符串结尾
    return (int)recved;
}

// 连接服务器（返回sockfd，失败返回-1）
int connect_to_server(const char* ip, int port) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket error");
        return -1;
    }
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip, &(server_addr.sin_addr)) != 1) {
        printf("inet_pton error\n");
        close(sockfd);
        return -1;
    }
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect error");
        close(sockfd);
        return -1;
    }
    return sockfd;
}