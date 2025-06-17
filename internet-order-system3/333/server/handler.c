// handler.c 服务端消息分发和业务处理实现

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "../common/protocol.h"
#include "handler.h"

// 商品、店铺、用户、订单文件名
#define GOODS_FILE "goods.dat"
#define STORE_FILE "store.dat"
#define USER_FILE "user.dat"
#define ORDER_FILE "order.dat"

// 自定义错误输出函数，便于调试
static void print_errno_error(const char* msg) {
    perror(msg);
}

// 工具函数：发送字符串回复
static void send_str(int sockfd, int cmd, const char* str) {
    ProtocolPacket p;
    p.cmd = cmd;
    strncpy(p.data, str, MAX_BUF-1);
    p.data[MAX_BUF-1] = '\0';
    send(sockfd, &p, sizeof(p), 0);
}

// 商品相关操作（仅示例，可根据需要扩展）
static void goods_add(const char* data, int sockfd) {
    Goods g;
    sscanf(data, "%[^|]|%[^|]|%lf|%d|%[^|]", g.name, g.category, &g.price, &g.stock, g.description);
    FILE* fp = fopen(GOODS_FILE, "ab+");
    if (!fp) { send_str(sockfd, CMD_GOODS_ADD, "商品添加失败！"); return; }
    fseek(fp, 0, SEEK_END);
    long len = ftell(fp);
    g.id = (int)(len/sizeof(Goods)) + 1;
    fwrite(&g, sizeof(Goods), 1, fp);
    fclose(fp);
    send_str(sockfd, CMD_GOODS_ADD, "商品添加成功！");
}

static void goods_view(int sockfd) {
    FILE* fp = fopen(GOODS_FILE, "rb");
    if (!fp) { send_str(sockfd, CMD_GOODS_VIEW, "暂无商品！"); return; }
    Goods g;
    char buf[MAX_BUF*4] = "";
    while (fread(&g, sizeof(Goods), 1, fp)) {
        char tmp[256];
        snprintf(tmp, sizeof(tmp), "ID:%d 名称:%s 分类:%s 价格:%.2lf 库存:%d 描述:%s\n", g.id, g.name, g.category, g.price, g.stock, g.description);
        strncat(buf, tmp, sizeof(buf)-strlen(buf)-1);
    }
    fclose(fp);
    send_str(sockfd, CMD_GOODS_VIEW, strlen(buf)?buf:"暂无商品！");
}

// 线程处理函数，主业务入口
void* handle_client(void* arg) {
    struct client_info* cinfo = (struct client_info*)arg;
    int client_sockfd = cinfo->client_sockfd;
    struct sockaddr_in client_addr = cinfo->client_addr;
    free(cinfo);

    // 获取客户端IP和端口
    char client_ip[INET_ADDRSTRLEN] = {0};
    inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));
    int client_port = ntohs(client_addr.sin_port);

    // 新的连接打印
    printf("新连接已建立，fd=%d，来自 %s:%d\n", client_sockfd, client_ip, client_port);

    while (1) {
        // 5. 开始通讯
        ProtocolPacket packet;
        ssize_t recv_len = recv(client_sockfd, &packet, sizeof(packet), 0);
        if (recv_len > 0) {
            // 根据命令类型分发处理
            switch (packet.cmd) {
                case CMD_EXIT:
                    printf("客户端请求退出，fd=%d，来自 %s:%d\n", client_sockfd, client_ip, client_port);
                    send_str(client_sockfd, CMD_EXIT, "服务器已处理退出请求。");
                    goto done;
                case CMD_GOODS_ADD:
                    goods_add(packet.data, client_sockfd);
                    break;
                case CMD_GOODS_VIEW:
                    goods_view(client_sockfd);
                    break;
                // TODO: 其它命令（商品增删改查/用户/店铺/订单等）在此扩展
                default:
                    send_str(client_sockfd, packet.cmd, "未知命令类型");
                    break;
            }
        } else if (recv_len == 0) {
            printf("连接断开，fd=%d，来自 %s:%d\n", client_sockfd, client_ip, client_port);
            break;
        } else {
            print_errno_error("recv failed");
            break;
        }
    }
done:
    // 6. 关闭连接
    close(client_sockfd); // 关闭与客户端的连接
    return NULL;
}