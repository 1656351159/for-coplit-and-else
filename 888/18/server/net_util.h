#ifndef NET_UTIL_H
#define NET_UTIL_H
int recvline(int sockfd, char *buf, int maxlen);
void sendline(int sockfd, const char *str);
#endif