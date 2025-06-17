#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // Linux下socket相关
#include <arpa/inet.h>
#include <sys/socket.h>
#include "menu.h"
#include "../common/protocol.h"

void input_str(const char* prompt, char* buf, int len) {
    printf("%s", prompt);
    if(fgets(buf, len, stdin)) {
        buf[strcspn(buf, "\n")] = 0;
    }
}

void send_packet(int sockfd, CmdType cmd, const char* data) {
    ProtocolPacket packet;
    memset(&packet, 0, sizeof(packet));
    packet.cmd = cmd;
    if(data) strncpy(packet.data, data, MAX_BUF-1);
    send(sockfd, &packet, sizeof(packet), 0);
}

void recv_print(int sockfd) {
    ProtocolPacket rp;
    int len = recv(sockfd, &rp, sizeof(rp), 0);
    if(len > 0) printf("%s\n", rp.data);
}

// 登录身份选择
void login_menu(char* role) {
    printf("请选择登录身份：\n1. 管理员(master)\n2. 商家(shoper)\n3. 顾客(customer)\n");
    char ch[8];
    fgets(ch, 8, stdin);
    if(ch[0]=='1') strcpy(role, "master");
    else if(ch[0]=='2') strcpy(role, "shoper");
    else strcpy(role, "customer");
}

// 管理员主循环
void master_loop(int sockfd) {
    while (1) {
        show_master_menu();
        char input[8];
        input_str("请输入编号: ", input, sizeof(input));
        if (strcmp(input, "0") == 0) { send_packet(sockfd, CMD_EXIT, NULL); break; }
        else if (strcmp(input, "1") == 0) {
            show_home_menu();
            input_str("首页编号: ", input, sizeof(input));
            // 此处可添加二级菜单交互
        }
        else if (strcmp(input, "2") == 0) { // 商品管理
            while (1) {
                show_master_goods_menu();
                input_str("商品管理编号: ", input, sizeof(input));
                if (strcmp(input, "0") == 0) break;
                else if (strcmp(input, "1") == 0) { // 添加商品
                    char name[64], cat[64], desc[128], price[32], stock[16], buf[MAX_BUF];
                    input_str("商品名: ", name, 64);
                    input_str("分类: ", cat, 64);
                    input_str("价格: ", price, 32);
                    input_str("库存: ", stock, 16);
                    input_str("描述: ", desc, 128);
                    snprintf(buf, MAX_BUF, "%s|%s|%s|%s|%s", name, cat, price, stock, desc);
                    send_packet(sockfd, CMD_GOODS_ADD, buf);
                    recv_print(sockfd);
                }
                else if (strcmp(input, "2") == 0) { // 删除商品
                    char id[16];
                    input_str("商品ID: ", id, 16);
                    send_packet(sockfd, CMD_GOODS_DEL, id);
                    recv_print(sockfd);
                }
                else if (strcmp(input, "3") == 0) { // 修改商品
                    char id[16], name[64], cat[64], desc[128], price[32], stock[16], buf[MAX_BUF];
                    input_str("商品ID: ", id, 16);
                    input_str("新商品名: ", name, 64);
                    input_str("新分类: ", cat, 64);
                    input_str("新价格: ", price, 32);
                    input_str("新库存: ", stock, 16);
                    input_str("新描述: ", desc, 128);
                    snprintf(buf, MAX_BUF, "%s|%s|%s|%s|%s|%s", id, name, cat, price, stock, desc);
                    send_packet(sockfd, CMD_GOODS_MODIFY, buf);
                    recv_print(sockfd);
                }
                else if (strcmp(input, "4") == 0) { // 查看商品
                    send_packet(sockfd, CMD_GOODS_VIEW, "");
                    recv_print(sockfd);
                }
            }
        }
        // ... 店铺管理、收藏、用户管理、订单管理、个人中心等可同理实现
    }
}

// 商家主循环
void shoper_loop(int sockfd) {
    while (1) {
        show_shoper_menu();
        char input[8];
        input_str("请输入编号: ", input, sizeof(input));
        if (strcmp(input, "0") == 0) { send_packet(sockfd, CMD_EXIT, NULL); break; }
        else if (strcmp(input, "2") == 0) { // 商品管理
            while (1) {
                show_shoper_goods_menu();
                input_str("商品管理编号: ", input, sizeof(input));
                if (strcmp(input, "0") == 0) break;
                // 其它操作同管理员
            }
        }
        // 订单管理、个人中心等同理
    }
}

// 顾客主循环
void customer_loop(int sockfd) {
    while (1) {
        show_customer_menu();
        char input[8];
        input_str("请输入编号: ", input, sizeof(input));
        if (strcmp(input, "0") == 0) { send_packet(sockfd, CMD_EXIT, NULL); break; }
        // 其它功能同理
    }
}

int main() {
    //1套接字
    int client_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_sockfd == -1) {
        perror("socket error");
        return -1;
    }

    //2.连接服务器
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
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

    char role[16] = {0};
    login_menu(role);
    if (strcmp(role, "master") == 0) master_loop(client_sockfd);
    else if (strcmp(role, "shoper") == 0) shoper_loop(client_sockfd);
    else customer_loop(client_sockfd);

    printf("客户端断开连接。\n");
    close(client_sockfd);
    return 0;
}