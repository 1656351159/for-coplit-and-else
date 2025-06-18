#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "../common/protocol.h"
#include "client_util.h"

#define SERVER_PORT 8080
#define SERVER_IP   "127.0.0.1"
#define MAX_INPUT   256

// 打印菜单
void print_menu() {
    printf("\n========== 菜单 ==========\n");
    printf("1. 注册\n");
    printf("2. 登录\n");
    printf("3. 查看用户列表\n");
    printf("4. 浏览所有商品\n");
    printf("0. 退出\n");
    printf("==========================\n");
}

// 菜单编号转命令号
int menu_to_cmd(int menu_id) {
    switch(menu_id) {
        case 1: return CMD_USER_REGISTER;
        case 2: return CMD_USER_LOGIN;
        case 3: return CMD_USER_VIEW;
        case 4: return CMD_GOODS_VIEW;
        case 0: return CMD_EXIT;
        default: return CMD_NONE;
    }
}

// 注册时输入用户信息
void fill_user_info(User* u) {
    memset(u, 0, sizeof(User));
    safe_input(u->username, sizeof(u->username), "用户名: ");
    safe_input(u->password, sizeof(u->password), "密码: ");
    safe_input(u->phone, sizeof(u->phone), "手机号: ");
    safe_input(u->email, sizeof(u->email), "邮箱: ");
    u->role = 0; // 普通用户
}

// 登录时输入用户名和密码
void fill_login_info(User* u) {
    memset(u, 0, sizeof(User));
    safe_input(u->username, sizeof(u->username), "用户名: ");
    safe_input(u->password, sizeof(u->password), "密码: ");
}

int main() {
    int client_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_sockfd == -1) {
        perror("socket error");
        return -1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &(server_addr.sin_addr)) != 1) {
        printf("inet_pton error\n");
        close(client_sockfd);
        return -1;
    }

    if (connect(client_sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect error");
        close(client_sockfd);
        return -1;
    }

    printf("连接服务器成功。\n");

    while (1) {
        print_menu();
        char input[MAX_INPUT];
        safe_input(input, sizeof(input), "请选择操作编号: ");
        int menu_id = atoi(input);
        int cmd = menu_to_cmd(menu_id);

        ProtocolPacket packet = {0};
        packet.cmd = cmd;

        // 根据不同命令准备数据
        if (cmd == CMD_USER_REGISTER) {
            User u;
            fill_user_info(&u);
            memcpy(packet.data, &u, sizeof(User));
        } else if (cmd == CMD_USER_LOGIN) {
            User u;
            fill_login_info(&u);
            memcpy(packet.data, &u, sizeof(User));
        }
        // 其它命令可以根据需要填写data

        // 发送协议包到服务器
        if (send(client_sockfd, &packet, sizeof(packet), 0) <= 0) {
            perror("send error");
            break;
        }

        // 接收服务器回复
        ProtocolPacket reply = {0};
        int n = recv(client_sockfd, &reply, sizeof(reply), 0);
        if (n > 0) {
            reply.data[MAX_REPLY_BUF-1] = '\0';
            printf("服务器回复: %s\n", reply.data);
        } else if (n == 0) {
            printf("服务器关闭了连接。\n");
            break;
        } else {
            perror("recv error");
            break;
        }

        if (cmd == CMD_EXIT) {
            printf("已退出客户端。\n");
            break;
        }
    }

    close(client_sockfd);
    return 0;
}