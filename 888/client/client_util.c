#include "client_util.h"
#include <stdio.h>
#include <string.h>

void safe_input(char *buf, int len, const char *prompt) {
    if (prompt) printf("%s", prompt);
    if (fgets(buf, len, stdin)) {
        buf[strcspn(buf, "\n")] = 0; // 去除换行
    }
}