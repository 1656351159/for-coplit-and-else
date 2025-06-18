#include "net_util.h"
#include <unistd.h>
#include <string.h>
int recvline(int sockfd, char *buf, int maxlen) {
    int n = 0;
    char c;
    while (n < maxlen - 1) {
        int r = read(sockfd, &c, 1);
        if (r <= 0) break;
        if (c == '\n') break;
        buf[n++] = c;
    }
    buf[n] = '\0';
    return n;
}
void sendline(int sockfd, const char *str) {
    write(sockfd, str, strlen(str));
    write(sockfd, "\n", 1);
}