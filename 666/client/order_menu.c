#include "order_menu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 假设当前登录用户ID，全局变量或登录时赋值
extern int current_user_id;

// 查看订单
void view_order() {
    FILE *fp = fopen("order.dat", "r");
    if (!fp) {
        printf("暂无订单信息。\n");
        return;
    }
    printf("订单ID\t用户ID\t商品ID\t数量\t状态\n");
    char line[128];
    int found = 0;
    int oid, uid, gid, qty;
    char status[32];
    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%d|%d|%d|%d|%31[^\n]", &oid, &uid, &gid, &qty, status) == 5) {
            // 显示所有订单（如只显示自己订单可加判断：if(uid == current_user_id)...）
            printf("%d\t%d\t%d\t%d\t%s\n", oid, uid, gid, qty, status);
            found = 1;
        }
    }
    fclose(fp);
    if (!found) printf("暂无订单。\n");
}

// 删除订单
void delete_order() {
    int order_id;
    printf("请输入要删除的订单ID: ");
    scanf("%d", &order_id); getchar();
    FILE *fp = fopen("order.dat", "r");
    if (!fp) {
        printf("订单不存在！\n");
        return;
    }
    FILE *tmp_fp = fopen("order.tmp", "w");
    char line[128];
    int found = 0, oid, uid, gid, qty;
    char status[32];
    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%d|%d|%d|%d|%31[^\n]", &oid, &uid, &gid, &qty, status) == 5) {
            if (oid == order_id /*&& uid == current_user_id*/) {
                found = 1;
                continue;
            }
            fputs(line, tmp_fp);
        }
    }
    fclose(fp);
    fclose(tmp_fp);
    remove("order.dat");
    rename("order.tmp", "order.dat");
    if (found)
        printf("删除订单成功！\n");
    else
        printf("未找到该订单ID！\n");
}

// 修改订单（仅状态字段，实际可扩展为修改数量等）
void modify_order() {
    int order_id;
    printf("请输入要修改的订单ID: ");
    scanf("%d", &order_id); getchar();
    FILE *fp = fopen("order.dat", "r");
    if (!fp) {
        printf("订单不存在！\n");
        return;
    }
    FILE *tmp_fp = fopen("order.tmp", "w");
    char line[128];
    int found = 0, oid, uid, gid, qty;
    char status[32];
    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%d|%d|%d|%d|%31[^\n]", &oid, &uid, &gid, &qty, status) == 5) {
            if (oid == order_id /*&& uid == current_user_id*/) {
                found = 1;
                printf("当前状态: %s\n", status);
                printf("请输入新状态: ");
                char new_status[32];
                if (fgets(new_status, sizeof(new_status), stdin)) {
                    new_status[strcspn(new_status, "\n")] = 0;
                    fprintf(tmp_fp, "%d|%d|%d|%d|%s\n", oid, uid, gid, qty, new_status);
                }
            } else {
                fputs(line, tmp_fp);
            }
        }
    }
    fclose(fp);
    fclose(tmp_fp);
    remove("order.dat");
    rename("order.tmp", "order.dat");
    if (found)
        printf("修改订单成功！\n");
    else
        printf("未找到该订单ID！\n");
}

// 订单菜单循环
void order_menu_loop(int sockfd) {
    while (1) {
        printf("【订单管理】\n");
        printf("1. 查看订单\n");
        printf("2. 修改订单\n");
        printf("3. 删除订单\n");
        printf("0. 返回上一级\n");
        char input[8];
        printf("订单菜单编号: ");
        if (!fgets(input, sizeof(input), stdin)) break;
        input[strcspn(input, "\n")] = 0;
        if (strcmp(input, "0") == 0) break;
        else if (strcmp(input, "1") == 0) view_order();
        else if (strcmp(input, "2") == 0) modify_order();
        else if (strcmp(input, "3") == 0) delete_order();
        else printf("无效输入，请重新输入。\n");
    }
}