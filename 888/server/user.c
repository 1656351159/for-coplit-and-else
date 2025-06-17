#include "user.h"
#include "net_util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define USER_FILE "data/user.dat"
#define MAX_LINE 256

// 用户信息结构体
typedef struct {
    char name[MAX_LINE];
    char password[MAX_LINE];
    char question[MAX_LINE];
    char answer[MAX_LINE];
    int role; // 1=管理员 2=商家 3=用户
} User;

// 查找用户，返回索引，没找到返回-1
int find_user(const char *name, User *u_list, int count) {
    for(int i=0; i < count; ++i) {
        if(strcmp(name, u_list[i].name) == 0) return i;
    }
    return -1;
}

// 读取所有用户，返回用户数
int load_users(User *u_list, int max_users) {
    FILE *fp = fopen(USER_FILE, "r");
    if (!fp) return 0;
    int n = 0;
    while (n < max_users && fscanf(fp, "%s %s %s %s %d", 
        u_list[n].name, u_list[n].password, u_list[n].question, u_list[n].answer, &u_list[n].role) == 5) {
        n++;
    }
    fclose(fp);
    return n;
}

// 写入所有用户
void save_users(User *u_list, int count) {
    FILE *fp = fopen(USER_FILE, "w");
    if (!fp) return;
    for(int i=0; i < count; ++i) {
        fprintf(fp, "%s %s %s %s %d\n", u_list[i].name, u_list[i].password, u_list[i].question, u_list[i].answer, u_list[i].role);
    }
    fclose(fp);
}

// 注册
void user_register(int sockfd, int role) {
    User users[1024];
    int count = load_users(users, 1024);
    char name[MAX_LINE], pwd[MAX_LINE], pwd2[MAX_LINE], ques[MAX_LINE], ans[MAX_LINE];

    sendline(sockfd, "请输入用户名：(已有用户？输入logon返回登录页面。)");
    recvline(sockfd, name, sizeof(name));
    if (strcmp(name, "logon") == 0) {
        user_login(sockfd, role);
        return;
    }
    if (find_user(name, users, count) != -1) {
        sendline(sockfd, "该用户名已存在，换一个吧。");
        user_register(sockfd, role);
        return;
    }
    sendline(sockfd, "请输入密码：");
    recvline(sockfd, pwd, sizeof(pwd));
    while(1) {
        sendline(sockfd, "请再次确认密码：");
        recvline(sockfd, pwd2, sizeof(pwd2));
        if(strcmp(pwd, pwd2) == 0) break;
        sendline(sockfd, "两次输入密码不一致，请重新输入。");
    }
    sendline(sockfd, "请设置密保问题：");
    recvline(sockfd, ques, sizeof(ques));
    sendline(sockfd, "请输入密保答案：");
    recvline(sockfd, ans, sizeof(ans));

    User u;
    strcpy(u.name, name);
    strcpy(u.password, pwd);
    strcpy(u.question, ques);
    strcpy(u.answer, ans);
    u.role = role;
    users[count++] = u;
    save_users(users, count);
    sendline(sockfd, "注册成功！");
}

// 登录
void user_login(int sockfd, int role) {
    User users[1024];
    int count = load_users(users, 1024);
    char name[MAX_LINE], pwd[MAX_LINE];

    sendline(sockfd, "请输入用户名：(没有用户？输入register返回注册页面。)");
    recvline(sockfd, name, sizeof(name));
    if (strcmp(name, "register") == 0) {
        user_register(sockfd, role);
        return;
    }
    int idx = find_user(name, users, count);
    if (idx == -1 || users[idx].role != role) {
        sendline(sockfd, "用户名不存在或角色不符。");
        user_login(sockfd, role);
        return;
    }
    int attempts = 0;
    while(attempts < 5) {
        sendline(sockfd, "请输入密码：（忘记密码输入change）");
        recvline(sockfd, pwd, sizeof(pwd));
        if (strcmp(pwd, "change") == 0) {
            user_forgot_pwd(sockfd, name, role);
            return;
        }
        if (strcmp(pwd, users[idx].password) == 0) {
            sendline(sockfd, "登录成功！");
            // 进入主菜单：可在此调用菜单分发
            return;
        }
        attempts++;
        sendline(sockfd, "密码错误，请重试。");
    }
    sendline(sockfd, "密码多次错误，自动返回主菜单。");
}

// 密码找回
void user_forgot_pwd(int sockfd, const char *name, int role) {
    User users[1024];
    int count = load_users(users, 1024);
    int idx = find_user(name, users, count);
    if (idx == -1 || users[idx].role != role) {
        sendline(sockfd, "用户不存在或角色不符。");
        return;
    }
    sendline(sockfd, users[idx].question);
    char ans[MAX_LINE];
    recvline(sockfd, ans, sizeof(ans));
    while (strcmp(ans, users[idx].answer) != 0) {
        sendline(sockfd, "密保答案错误，请重试。");
        recvline(sockfd, ans, sizeof(ans));
    }
    sendline(sockfd, "请输入新密码：");
    recvline(sockfd, users[idx].password, sizeof(users[idx].password));
    save_users(users, count);
    sendline(sockfd, "密码重置成功，请重新登录。");
    user_login(sockfd, role);
}