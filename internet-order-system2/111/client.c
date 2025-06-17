#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // Linux下socket相关
#include <arpa/inet.h>
#include <sys/socket.h>

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
	
    //3. 开始通讯send recv
    while (1) {
        char buf[1024];

        printf("请输入消息,如果想要退出,请输入exit:");
        if (fgets(buf, sizeof(buf), stdin) == NULL) {
            printf("fgets error or EOF\n");
            break;
        }
        //去掉换行符
        buf[strcspn(buf, "\n")] = 0;

        // 如果输入空字符串，提示并继续
        if (strlen(buf) == 0) {
            printf("不能发送空消息，请重新输入！\n");
            continue;
        }
            // 新增：输入exit断开连接
    if (strcmp(buf, "exit") == 0) {
        printf("客户端即将断开连接。\n");
        break;
    }

        //发送数据
        if (send(client_sockfd, buf, strlen(buf), 0) == -1) {
            perror("send error");
            break;
        }

        //接收数据
        int len = recv(client_sockfd, buf, sizeof(buf) - 1, 0); // 只读1023字节，留1字节给\0
        if (len <= 0) {
            printf("recv error\n");
            break;
        }
        buf[len] = '\0'; // 确保字符串以null结尾
        printf("服务器回复: %s\n", buf);
    }

    //4关闭套接字
    printf("客户端断开连接。\n");
    close(client_sockfd);
    return 0;
}
