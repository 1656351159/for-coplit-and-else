#include "profile_menu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 假设当前登录用户ID和用户名、联系方式
extern int current_user_id;

// 个人信息结构体
typedef struct {
    int id;
    char username[32];
    char phone[32];
    char email[64];
} UserProfile;

// 从 user.dat 读取当前用户信息
int load_profile(UserProfile *profile) {
    FILE *fp = fopen("user.dat", "r");
    if (!fp) return 0;
    char line[256];
    int found = 0;
    while (fgets(line, sizeof(line), fp)) {
        int id;
        char username[32], phone[32], email[64];
        if (sscanf(line, "%d|%31[^|]|%31[^|]|%63[^\n]", &id, username, phone, email) == 4) {
            if (id == current_user_id) {
                profile->id = id;
                strcpy(profile->username, username);
                strcpy(profile->phone, phone);
                strcpy(profile->email, email);
                found = 1;
                break;
            }
        }
    }
    fclose(fp);
    return found;
}

// 覆盖 user.dat 修改用户信息
void save_profile(const UserProfile *profile) {
    FILE *fp = fopen("user.dat", "r");
    if (!fp) {
        printf("无法打开user.dat\n");
        return;
    }
    FILE *tmp_fp = fopen("user.tmp", "w");
    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        int id;
        char username[32], phone[32], email[64];
        if (sscanf(line, "%d|%31[^|]|%31[^|]|%63[^\n]", &id, username, phone, email) == 4) {
            if (id == profile->id) {
                fprintf(tmp_fp, "%d|%s|%s|%s\n", profile->id, profile->username, profile->phone, profile->email);
            } else {
                fputs(line, tmp_fp);
            }
        }
    }
    fclose(fp);
    fclose(tmp_fp);
    remove("user.dat");
    rename("user.tmp", "user.dat");
}

// 查看个人信息
void view_profile() {
    UserProfile profile;
    if (!load_profile(&profile)) {
        printf("未找到个人信息！\n");
        return;
    }
    printf("【个人信息】\n");
    printf("用户ID: %d\n用户名: %s\n电话: %s\n邮箱: %s\n", profile.id, profile.username, profile.phone, profile.email);
}

// 修改个人信息
void modify_profile() {
    UserProfile profile;
    if (!load_profile(&profile)) {
        printf("未找到个人信息！\n");
        return;
    }
    printf("修改个人信息（回车保留原值）：\n");
    printf("原用户名: %s 新用户名: ", profile.username);
    char buf[64];
    if (fgets(buf, sizeof(buf), stdin)) {
        buf[strcspn(buf, "\n")] = 0;
        if (strlen(buf) > 0) strcpy(profile.username, buf);
    }
    printf("原电话: %s 新电话: ", profile.phone);
    if (fgets(buf, sizeof(buf), stdin)) {
        buf[strcspn(buf, "\n")] = 0;
        if (strlen(buf) > 0) strcpy(profile.phone, buf);
    }
    printf("原邮箱: %s 新邮箱: ", profile.email);
    if (fgets(buf, sizeof(buf), stdin)) {
        buf[strcspn(buf, "\n")] = 0;
        if (strlen(buf) > 0) strcpy(profile.email, buf);
    }
    save_profile(&profile);
    printf("修改成功！\n");
}

// 个人中心菜单循环
void profile_menu_loop(int sockfd) {
    while (1) {
        printf("【个人中心】\n");
        printf("1. 查看个人信息\n");
        printf("2. 修改个人信息\n");
        printf("0. 返回上一级\n");
        char input[8];
        printf("菜单编号: ");
        if (!fgets(input, sizeof(input), stdin)) break;
        input[strcspn(input, "\n")] = 0;
        if (strcmp(input, "0") == 0) break;
        else if (strcmp(input, "1") == 0) view_profile();
        else if (strcmp(input, "2") == 0) modify_profile();
        else printf("无效输入，请重新输入。\n");
    }
}