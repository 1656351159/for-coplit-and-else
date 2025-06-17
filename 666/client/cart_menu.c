#include "cart_menu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 假设当前登录用户ID，全局变量或登录时赋值
extern int current_user_id;

// 添加商品到购物车
void add_to_cart() {
    int goods_id, count;
    printf("请输入要添加的商品ID: ");
    scanf("%d", &goods_id); getchar();
    printf("请输入购买数量: ");
    scanf("%d", &count); getchar();

    // 检查是否已在购物车，若有则数量叠加
    FILE *fp = fopen("cart.dat", "r");
    FILE *tmp_fp = fopen("cart.tmp", "w");
    int found = 0;
    char line[64];
    int uid, gid, qty;
    if (fp) {
        while (fgets(line, sizeof(line), fp)) {
            if (sscanf(line, "%d|%d|%d", &uid, &gid, &qty) == 3) {
                if (uid == current_user_id && gid == goods_id) {
                    qty += count;
                    found = 1;
                }
                fprintf(tmp_fp, "%d|%d|%d\n", uid, gid, qty);
            }
        }
        fclose(fp);
    }
    if (!found) // 新商品
        fprintf(tmp_fp, "%d|%d|%d\n", current_user_id, goods_id, count);
    fclose(tmp_fp);

    remove("cart.dat");
    rename("cart.tmp", "cart.dat");
    printf("添加到购物车成功！\n");
}

// 查看购物车
void view_cart() {
    FILE *fp = fopen("cart.dat", "r");
    if (!fp) {
        printf("购物车为空！\n");
        return;
    }
    printf("你的购物车如下：\n商品ID\t数量\n");
    char line[64];
    int uid, gid, qty, found = 0;
    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%d|%d|%d", &uid, &gid, &qty) == 3) {
            if (uid == current_user_id) {
                printf("%d\t%d\n", gid, qty);
                found = 1;
            }
        }
    }
    fclose(fp);
    if (!found)
        printf("购物车为空！\n");
}

// 删除购物车商品
void remove_from_cart() {
    int goods_id;
    printf("请输入要删除的商品ID: ");
    scanf("%d", &goods_id); getchar();
    FILE *fp = fopen("cart.dat", "r");
    if (!fp) {
        printf("购物车为空！\n");
        return;
    }
    FILE *tmp_fp = fopen("cart.tmp", "w");
    char line[64];
    int uid, gid, qty, found = 0;
    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%d|%d|%d", &uid, &gid, &qty) == 3) {
            if (uid == current_user_id && gid == goods_id) {
                found = 1;
                continue;
            }
            fprintf(tmp_fp, "%d|%d|%d\n", uid, gid, qty);
        }
    }
    fclose(fp);
    fclose(tmp_fp);
    remove("cart.dat");
    rename("cart.tmp", "cart.dat");
    if (found)
        printf("删除成功！\n");
    else
        printf("未找到该商品ID！\n");
}

// 下单结算
void checkout_cart() {
    FILE *fp = fopen("cart.dat", "r");
    if (!fp) {
        printf("购物车为空，无法结算！\n");
        return;
    }
    FILE *tmp_fp = fopen("cart.tmp", "w");
    char line[64];
    int uid, gid, qty, found = 0;
    printf("正在结算...\n");
    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%d|%d|%d", &uid, &gid, &qty) == 3) {
            if (uid == current_user_id) {
                // 这里可以生成订单，比如写入 order.dat 等
                printf("下单：商品ID %d 数量 %d\n", gid, qty);
                found = 1;
                // 不写入临时文件，实现结算后购物车清空
            } else {
                fprintf(tmp_fp, "%d|%d|%d\n", uid, gid, qty);
            }
        }
    }
    fclose(fp);
    fclose(tmp_fp);
    remove("cart.dat");
    rename("cart.tmp", "cart.dat");
    if (found)
        printf("结算成功，购物车已清空！\n");
    else
        printf("购物车为空！\n");
}

// 购物车菜单循环
void cart_menu_loop(int sockfd) {
    while (1) {
        printf("【购物车管理】\n");
        printf("1. 查看购物车\n");
        printf("2. 添加商品到购物车\n");
        printf("3. 删除购物车商品\n");
        printf("4. 下单结算\n");
        printf("0. 返回上一级\n");
        char input[8];
        printf("购物车菜单编号: ");
        if (!fgets(input, sizeof(input), stdin)) break;
        input[strcspn(input, "\n")] = 0;
        if (strcmp(input, "0") == 0) break;
        else if (strcmp(input, "1") == 0) view_cart();
        else if (strcmp(input, "2") == 0) add_to_cart();
        else if (strcmp(input, "3") == 0) remove_from_cart();
        else if (strcmp(input, "4") == 0) checkout_cart();
        else printf("无效输入，请重新输入。\n");
    }
}