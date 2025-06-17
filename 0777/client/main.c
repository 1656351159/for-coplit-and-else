#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // Linux下socket相关
#include <arpa/inet.h>
#include <sys/socket.h>
#include "client_util.h"
#include "menu.h" // 菜单头文件，需自己创建
#include "common.h"
int main() {
    //1套接字
    int client_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_sockfd == -1) {
        perror("socket error");
        return -1;
    }

    //2.连接服务器
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET; // IPv4协议
    server_addr.sin_port = htons(8080); // 端口号，使用htons将主机字节序转换为网络字节序
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
    // 3. 调用身份选择和登录/注册功能
    start(); // 调用 common.h 中的 start() 函数，显示身份选择和登录/注册界面

    // 4. 进入菜单主循环
    int identity = 0;
    printf("请选择身份：1.管理员 2.商家 3.顾客\n");
    scanf("%d", &identity);
    getchar(); // 清除输入缓冲区
    
    if (identity == 1) {
        master_loop(client_sockfd); // 管理员菜单主循环
    } else if (identity == 2) {
        shoper_loop(client_sockfd); // 商家菜单主循环
    } else if (identity == 3) {
        customer_loop(client_sockfd); // 顾客菜单主循环
    }

    //4关闭套接字
    printf("客户端断开连接。\n");
    close(client_sockfd);
    return 0;
}