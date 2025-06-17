// 客户端通用工具头文件 client_util.h
#ifndef CLIENT_UTIL_H
#define CLIENT_UTIL_H

#include <stddef.h>

// 输入字符串（带提示，去除换行符）
void input_str(const char* prompt, char* buf, int size);

// 发送数据（简单包装，可扩展成带协议的发送）
int send_data(int sockfd, const void* buf, size_t len);

// 接收数据（简单包装）
int recv_data(int sockfd, void* buf, size_t maxlen);

// 连接服务器（返回sockfd，失败返回-1）
int connect_to_server(const char* ip, int port);

#endif // CLIENT_UTIL_H