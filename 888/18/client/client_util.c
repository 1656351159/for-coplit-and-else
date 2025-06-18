#include <stdio.h>
#include <string.h>
#include "client_util.h"

void safe_input(char* buf, size_t size, const char* prompt) {
    printf("%s", prompt);
    if (fgets(buf, size, stdin)) {
        size_t len = strlen(buf);
        if (len > 0 && buf[len-1] == '\n') buf[len-1] = '\0';
    } else {
        buf[0] = '\0';
    }
}