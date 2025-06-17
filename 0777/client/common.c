#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_LINE_LENGTH 256
#define MAX_NAME_LENGTH 64
#define MAX_ADDRESS_LENGTH 128
#define MAX_PHONE_LENGTH 32
#define MAX_QUESTION_LENGTH 256
#define MAX_ANSWER_LENGTH 256

FILE *readFile;
FILE *outFile;
FILE *roFile;

void log_on(void);
void start(void);
void rgs(void);
void confirm(const char *name, const char *password);
void change(const char *name);

void change(const char *name) {
    roFile = fopen("log_on.txt", "r+");
    if (roFile == NULL) {
        printf("无法打开文件 log_on.txt\n");
        return;
    }

    readFile = fopen("log_on.txt", "r");
    if (readFile == NULL) {
        printf("无法打开文件 log_on.txt\n");
        fclose(roFile);
        return;
    }

    char line[MAX_LINE_LENGTH];
    char sss[MAX_LINE_LENGTH];
    bool found = false;

    while (fgets(line, sizeof(line), readFile)) {
        line[strcspn(line, "\n")] = '\0'; // 移除换行符
        if (strcmp(line, name) == 0) {
            found = true;
            printf("请回答以下问题\n");
            
            // 跳过密码行
            if (!fgets(line, sizeof(line), readFile)) break;
            
            // 获取问题
            if (!fgets(line, sizeof(line), readFile)) break;
            line[strcspn(line, "\n")] = '\0';
            printf("%s\n", line);
            
            // 获取答案
            if (!fgets(line, sizeof(line), readFile)) break;
            line[strcspn(line, "\n")] = '\0';
            char ans[MAX_ANSWER_LENGTH];
            scanf("%s", ans);
            
            while (strcmp(ans, line) != 0) {
                printf("回答错误！请再次尝试！\n");
                scanf("%s", ans);
            }

            rewind(roFile);
            while (fgets(sss, sizeof(sss), roFile) ){
                sss[strcspn(sss, "\n")] = '\0';
                if (strcmp(sss, name) == 0) {
                    // 跳过用户名行
                    if (!fgets(sss, sizeof(sss), roFile)) break;
                    
                    long pos = ftell(roFile);
                    fseek(roFile, pos - strlen(sss) - 1, SEEK_SET);
                    
                    printf("请输入新的密码：\n");
                    char new_password[MAX_LINE_LENGTH];
                    scanf("%s", new_password);
                    fprintf(roFile, "%s\n", new_password);
                    
                    fclose(roFile);
                    fclose(readFile);
                    log_on();
                    return;
                }
            }
        }
    }

    if (!found) {
        printf("未找到用户 %s\n", name);
    }

    fclose(roFile);
    fclose(readFile);
}

void confirm(const char *name, const char *password) {
    char confirm_password[MAX_LINE_LENGTH];
    printf("请再次确认密码：\n");
    scanf("%s", confirm_password);
    
    while (strcmp(password, confirm_password) != 0) {
        printf("确认密码与注册密码不同，请再次尝试。\n");
        printf("请再次确认密码：（修改注册密码请输入back）\n");
        scanf("%s", confirm_password);
        
        if (strcmp(confirm_password, "back") == 0) {
            rgs();
            return;
        }
    }
    
    printf("为了您账号的安全着想，请设置密保问题，以便于您以后找回密码。\n");
    char question[MAX_QUESTION_LENGTH];
    char answer[MAX_ANSWER_LENGTH];
    
    printf("请输入Q：");
    scanf("%s", question);
    printf("请输入A：");
    scanf("%s", answer);
    
    outFile = fopen("log_on.txt", "a");
    if (outFile == NULL) {
        printf("无法打开文件 log_on.txt\n");
        return;
    }
    
    fprintf(outFile, "%s\n%s\n%s\n%s\n", name, password, question, answer);
    printf("注册成功！\n");
    fclose(outFile);
}

void rgs() {
    printf("请输入用户名：(已有用户？输入logon返回登录页面。)\n");
    char name[MAX_NAME_LENGTH];
    scanf("%s", name);
    
    if (strcmp(name, "logon") == 0) {
        log_on();
        return;
    }
    
    readFile = fopen("log_on.txt", "r");
    if (readFile == NULL) {
        printf("无法打开文件 log_on.txt\n");
        return;
    }
    
    char line[MAX_LINE_LENGTH];
    bool exists = false;
    
    while (fgets(line, sizeof(line), readFile)) {
        line[strcspn(line, "\n")] = '\0';
        if (strcmp(line, name) == 0) {
            exists = true;
            break;
        }
    }
    
    fclose(readFile);
    
    if (exists) {
        printf("该用户已注册！\n");
        rgs();
        return;
    }
    
    printf("请输入密码：\n");
    char password[MAX_LINE_LENGTH];
    scanf("%s", password);
    
    confirm(name, password);
}

void log_on() {
    printf("请输入用户名：(没有用户？输入register返回注册页面。)\n");
    char name[MAX_NAME_LENGTH];
    scanf("%s", name);
    
    if (strcmp(name, "register") == 0) {
        rgs();
        return;
    }
    
    readFile = fopen("log_on.txt", "r");
    if (readFile == NULL) {
        printf("无法打开文件 log_on.txt\n");
        return;
    }
    
    char line[MAX_LINE_LENGTH];
    bool found = false;
    
    while (fgets(line, sizeof(line), readFile)) {
        line[strcspn(line, "\n")] = '\0';
        if (strcmp(line, name) == 0) {
            found = true;
            printf("请输入密码：\n");
            
            if (!fgets(line, sizeof(line), readFile)) break;
            line[strcspn(line, "\n")] = '\0';
            
            char password[MAX_LINE_LENGTH];
            scanf("%s", password);
            
            int attempts = 0;
            while (strcmp(password, line) != 0) {
                if (attempts >= 5) {
                    printf("密码多次错误！系统已锁定，欢迎您下次光临。");
                    fclose(readFile);
                    start();
                    return;
                } else {
                    printf("密码错误！请再次尝试！（忘记密码？输入change修改密码。）\n");
                    scanf("%s", password);
                    
                    if (strcmp(password, "change") == 0) {
                        fclose(readFile);
                        change(name);
                        return;
                    }
                }
                attempts++;
            }
            
            printf("           欢迎用户回家!       \n");
            fclose(readFile);
            return;
        }
    }
    
    if (!found) {
        printf("该用户名不存在，请重试！（如需注册请输入register）\n");
        fclose(readFile);
        log_on();
        return;
    }
    
    fclose(readFile);
}

void start() {
    printf("=============================================\n");
    printf("       欢迎来到拼好饭点餐系统，请问您是？\n");
    printf("      1.管理者        2.商家       3.用户\n");
    printf("=============================================\n");
    
    int choice;
    scanf("%d", &choice);
    
    if (choice == 1) {
        printf("=============================================\n");
        printf("        尊贵的管理者，请问有什么可以帮您？\n");
        printf("            1.登录        2.注册\n");
        printf("=============================================\n");
    } else if (choice == 2) {
        printf("=============================================\n");
        printf("         尊贵的商家，请问有什么可以帮您？\n");
        printf("            1.登录        2.注册\n");
        printf("=============================================\n");
    } else if (choice == 3) {
        printf("=============================================\n");
        printf("         尊贵的用户，请问有什么可以帮您？\n");
        printf("            1.登录        2.注册\n");
        printf("=============================================\n");
    }
    
    int sub_choice;
    scanf("%d", &sub_choice);
    
    if (sub_choice == 1) {
        log_on();
    } else {
        rgs();
    }
}