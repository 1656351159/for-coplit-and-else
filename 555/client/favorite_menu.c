#include "favorite_menu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 假设当前登录用户ID，全局变量或登录后赋值
extern int current_user_id;

// 增加收藏
void add_favorite() {
    int goods_id;
    printf("请输入要收藏的商品ID: ");
    scanf("%d", &goods_id); getchar();
    // 检查是否已收藏
    FILE *fp = fopen("favorite.dat", "r");
    if (fp) {
        char line[64];
        int uid, gid;
        while (fgets(line, sizeof(line), fp)) {
            if (sscanf(line, "%d|%d", &uid, &gid) == 2) {
                if (uid == current_user_id && gid == goods_id) {
                    printf("该商品已收藏，无需重复收藏！\n");
                    fclose(fp);
                    return;
                }
            }
        }
        fclose(fp);
    }
    // 添加收藏
    fp = fopen("favorite.dat", "a");
    if (!fp) {
        printf("无法打开favorite.dat文件！\n");
        return;
    }
    fprintf(fp, "%d|%d\n", current_user_id, goods_id);
    fclose(fp);
    printf("收藏成功！\n");
}

// 取消收藏
void remove_favorite() {
    int goods_id;
    printf("请输入要取消收藏的商品ID: ");
    scanf("%d", &goods_id); getchar();
    FILE *fp = fopen("favorite.dat", "r");
    if (!fp) {
        printf("收藏列表为空！\n");
        return;
    }
    FILE *tmp_fp = fopen("favorite.tmp", "w");
    char line[64];
    int uid, gid, found = 0;
    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%d|%d", &uid, &gid) == 2) {
            if (uid == current_user_id && gid == goods_id) {
                found = 1;
                continue;
            }
        }
        fputs(line, tmp_fp);
    }
    fclose(fp);
    fclose(tmp_fp);
    remove("favorite.dat");
    rename("favorite.tmp", "favorite.dat");
    if (found)
        printf("取消收藏成功！\n");
    else
        printf("未找到该收藏项！\n");
}

// 查看收藏
void view_favorite() {
    FILE *fp = fopen("favorite.dat", "r");
    if (!fp) {
        printf("收藏列表为空！\n");
        return;
    }
    printf("你的收藏商品ID如下：\n");
    char line[64];
    int uid, gid, found = 0;
    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%d|%d", &uid, &gid) == 2) {
            if (uid == current_user_id) {
                printf("商品ID: %d\n", gid);
                found = 1;
            }
        }
    }
    fclose(fp);
    if (!found)
        printf("暂无收藏。\n");
}

// 收藏菜单循环
void favorite_menu_loop(int sockfd) {
    while (1) {
        printf("【收藏管理】\n");
        printf("1. 查看收藏\n");
        printf("2. 收藏商品\n");
        printf("3. 取消收藏\n");
        printf("0. 返回上一级\n");
        char input[8];
        printf("收藏菜单编号: ");
        if (!fgets(input, sizeof(input), stdin)) break;
        input[strcspn(input, "\n")] = 0;
        if (strcmp(input, "0") == 0) break;
        else if (strcmp(input, "1") == 0) view_favorite();
        else if (strcmp(input, "2") == 0) add_favorite();
        else if (strcmp(input, "3") == 0) remove_favorite();
        else printf("无效输入，请重新输入。\n");
    }
}