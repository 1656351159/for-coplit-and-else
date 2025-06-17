#ifndef CLIENT_UTIL_H
#define CLIENT_UTIL_H

// 安全输入字符串，去除回车，支持提示
void safe_input(char *buf, int len, const char *prompt);

#endif