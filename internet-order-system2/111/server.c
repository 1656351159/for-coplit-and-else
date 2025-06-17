#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>           // Linux下socket相关函数和多线程
#include <pthread.h>
#include <arpa/inet.h>
#include <netinet/in.h>

// 自定义错误输出函数，便于调试
void print_errno_error(const char* msg) {
    printf("%s: errno %d (%s)\n", msg, errno, strerror(errno));
}

// 结构体：用于传递客户端socket和地址
struct client_info {
    int client_sockfd;
    struct sockaddr_in client_addr;
};

// 线程处理函数
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
        char buffer[1024] = { 0 };
        // Linux下recv返回ssize_t类型
        ssize_t recv_len = recv(client_sockfd, buffer, sizeof(buffer) - 1, 0);
        if (recv_len > 0) {
            buffer[recv_len] = '\0'; // 确保字符串结尾
            printf("来自客户端%s:%d(fd=%d)的消息: %s\n", client_ip, client_port, client_sockfd, buffer);

            // 回显给客户端
            if (send(client_sockfd, buffer, strlen(buffer), 0) == -1) {
                print_errno_error("send failed");
            }
        }
        else if (recv_len == 0) {
            printf("连接断开，fd=%d，来自 %s:%d\n", client_sockfd, client_ip, client_port); // 连接断开打印
            break;
        }
        else {
            print_errno_error("recv failed");
            break;
        }
    }

    // 6. 关闭连接
    close(client_sockfd); // 关闭与客户端的连接
    return NULL;
}

int main() {
    // 1. 创建socket套接字
    /* socket函数的参数说明：
       协议地址簇（如AF_INET表示IPv4）
       类型（如SOCK_STREAM表示流式协议，SOCK_DGRAM表示数据报协议）
       协议（通常为0，表示自动选择合适的协议）
    */
    int listen_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sockfd == -1) {
        print_errno_error("socket creation failed");
        exit(EXIT_FAILURE);
    }
    // perror是一个标准库函数，用于打印最近一次系统调用失败的错误信息。
    // Linux下socket相关错误可以直接用perror或errno

    // 允许端口快速重用
    int opt = 1;
    setsockopt(listen_sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // 2. 绑定端口
    struct sockaddr_in local = { 0 };
    local.sin_family = AF_INET; // IPv4协议
    local.sin_port = htons(8080); // 端口号，使用htons将主机字节序转换为网络字节序
    local.sin_addr.s_addr = INADDR_ANY; // 服务端，接受所有IP的连接

    // bind函数将socket与本地地址（IP和端口）绑定
    if (bind(listen_sockfd, (struct sockaddr*)&local, sizeof(local)) == -1) {
        print_errno_error("bind failed");
        close(listen_sockfd);
        exit(EXIT_FAILURE);
    }
    // 绑定成功后，socket就有了自己的地址，可以接受客户端的连接请求

    // 3. 给socket开启监听属性
    // listen函数将主动套接字转为被动套接字，准备接受连接
    if (listen(listen_sockfd, 10) == -1) {
        print_errno_error("listen failed");
        close(listen_sockfd);
        exit(EXIT_FAILURE);
    }
    // 第二个参数为等待队列的最大长度

    printf("服务器启动，监听端口8080...\n");

    while (1) { // 支持多次连接
        // 4. 等待客户端连接
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int client_sockfd = accept(listen_sockfd, (struct sockaddr*)&client_addr, &client_addr_len); // 客户端
        if (client_sockfd == -1) {
            print_errno_error("accept failed");
            continue;
        }
        // 分配结构体传递socket和地址
        struct client_info* cinfo = malloc(sizeof(struct client_info));
        if (!cinfo) {
            print_errno_error("malloc failed");
            close(client_sockfd);
            continue;
        }
        cinfo->client_sockfd = client_sockfd;
        cinfo->client_addr = client_addr;

        // 新的连接打印在handle_client线程里

        // 创建线程处理客户端
        pthread_t tid;
        if (pthread_create(&tid, NULL, handle_client, cinfo) != 0) {
            print_errno_error("pthread_create failed");
            close(client_sockfd);
            free(cinfo);
            continue;
        }
        pthread_detach(tid); // 自动回收线程资源
    }

    close(listen_sockfd); // 关闭监听socket
    return 0;
}
