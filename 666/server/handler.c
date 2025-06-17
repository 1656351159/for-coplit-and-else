#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "../common/protocol.h"
#include "handler.h"
#include "data_store.h"

static void send_str(int sockfd, int cmd, const char* str) {
    ProtocolPacket p;
    p.cmd = cmd;
    strncpy(p.data, str, MAX_BUF-1);
    p.data[MAX_BUF-1] = '\0';
    send(sockfd, &p, sizeof(p), 0);
}

void* handle_client(void* arg) {
    struct client_info* cinfo = (struct client_info*)arg;
    int client_sockfd = cinfo->client_sockfd;
    struct sockaddr_in client_addr = cinfo->client_addr;
    free(cinfo);

    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));
    int client_port = ntohs(client_addr.sin_port);

    printf("新连接已建立，fd=%d，来自 %s:%d\n", client_sockfd, client_ip, client_port);

    while (1) {
        ProtocolPacket packet;
        ssize_t recv_len = recv(client_sockfd, &packet, sizeof(packet), 0);
        if (recv_len > 0) {
            switch (packet.cmd) {
                case CMD_EXIT:
                    printf("客户端请求退出，fd=%d，来自 %s:%d\n", client_sockfd, client_ip, client_port);
                    send_str(client_sockfd, CMD_EXIT, "服务器已处理退出请求。");
                    goto done;
                case CMD_GOODS_ADD:
                    // goods_add(packet.data, client_sockfd); // 你可以按需补充
                    send_str(client_sockfd, CMD_GOODS_ADD, "商品添加功能未实现");
                    break;
                case CMD_GOODS_VIEW:
                    // goods_view(client_sockfd); // 你可以按需补充
                    send_str(client_sockfd, CMD_GOODS_VIEW, "商品浏览功能未实现");
                    break;
                default:
                    send_str(client_sockfd, packet.cmd, "未知命令类型");
                    break;
            }
        } else if (recv_len == 0) {
            printf("连接断开，fd=%d，来自 %s:%d\n", client_sockfd, client_ip, client_port);
            break;
        } else {
            perror("recv failed");
            break;
        }
    }
done:
    close(client_sockfd);
    return NULL;
}