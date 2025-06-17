#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "client_util.h"

#define BUFSIZE 2048

// 从服务端接收菜单内容并显示
void show_menu_from_server(int sockfd) {
    char buffer[BUFSIZE];
    int n;
    // 循环接收，直到遇到"END\n"标志
    while ((n = recv(sockfd, buffer, sizeof(buffer)-1, 0)) > 0) {
        buffer[n] = '\0';
        if (strcmp(buffer, "END\n") == 0) break;
        printf("%s", buffer);
        // 若遇到END\n在数据中间，也能break
        if (strstr(buffer, "END\n")) break;
    }
}

int main() {
    int client_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_sockfd == -1) {
        perror("socket error");
        return -1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080); // 服务端端口
    if (inet_pton(AF_INET, "127.0.0.1", &(server_addr.sin_addr)) != 1) {
        printf("inet_pton error\n");
        close(client_sockfd);
        return -1;
    }

    if (connect(client_sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect error");
        close(client_sockfd);
        return -1;
    }

    while (1) {
        show_menu_from_server(client_sockfd);

        char input[64];
        safe_input(input, sizeof(input), "请输入编号(输入0退出): ");
        send(client_sockfd, input, strlen(input), 0);

        if (strcmp(input, "0") == 0) break;
    }

    printf("客户端断开连接。\n");
    close(client_sockfd);
    return 0;
}