#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// 最大长度定义
#define MAX_LINE_LENGTH 256
#define MAX_NAME_LENGTH 64
#define MAX_ADDRESS_LENGTH 128
#define MAX_PHONE_LENGTH 32
#define MAX_QUESTION_LENGTH 256
#define MAX_ANSWER_LENGTH 256

// 全局文件指针声明
extern FILE *readFile;
extern FILE *outFile;
extern FILE *roFile;

// 函数声明
void log_on(void);
void start(void);
void rgs(void);
void confirm(const char *name, const char *password);
void change(const char *name);

#endif /* COMMON_H */