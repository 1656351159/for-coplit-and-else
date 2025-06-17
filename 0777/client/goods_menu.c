#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "goods_menu.h"

// 获取下一个商品ID
int get_next_goods_id() {
    FILE *fp = fopen("goods.dat", "r");
    if (!fp) return 1;
    int last_id = 0;
    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        int id = 0;
        sscanf(line, "%d|", &id);
        if (id > last_id) last_id = id;
    }
    fclose(fp);
    return last_id + 1;
}

// 添加商品
void add_goods() {
    char name[64];
    int category, stock;
    float price;
    printf("请输入商品名称: ");
    if (!fgets(name, sizeof(name), stdin)) return;
    name[strcspn(name, "\n")] = 0;

    printf("请输入类别编号(1中餐 2西餐 3快餐 4饮品 5拼好饭): ");
    scanf("%d", &category); getchar();
    printf("请输入价格: ");
    scanf("%f", &price); getchar();
    printf("请输入库存: ");
    scanf("%d", &stock); getchar();

    int id = get_next_goods_id();
    FILE *fp = fopen("goods.dat", "a");
    if (!fp) {
        printf("无法打开goods.dat文件！\n");
        return;
    }
    fprintf(fp, "%d|%s|%d|%.2f|%d\n", id, name, category, price, stock);
    fclose(fp);
    printf("添加商品成功，商品ID：%d\n", id);
}

// 查看所有商品
void view_goods() {
    FILE *fp = fopen("goods.dat", "r");
    if (!fp) {
        printf("暂无商品信息。\n");
        return;
    }
    printf("商品列表：\nID\t名称\t类别\t价格\t库存\n");
    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        int id, category, stock;
        float price;
        char name[64];
        if (sscanf(line, "%d|%63[^|]|%d|%f|%d", &id, name, &category, &price, &stock) == 5) {
            printf("%d\t%s\t%d\t%.2f\t%d\n", id, name, category, price, stock);
        }
    }
    fclose(fp);
}

// 删除商品
void delete_goods() {
    view_goods();
    printf("请输入要删除的商品ID: ");
    char buf[16];
    if (!fgets(buf, sizeof(buf), stdin)) return;
    int del_id = atoi(buf);

    FILE *fp = fopen("goods.dat", "r");
    if (!fp) {
        printf("goods.dat不存在！\n");
        return;
    }
    FILE *fp_tmp = fopen("goods.tmp", "w");
    if (!fp_tmp) {
        fclose(fp);
        printf("无法创建临时文件！\n");
        return;
    }
    char line[256];
    int found = 0;
    while (fgets(line, sizeof(line), fp)) {
        int id;
        sscanf(line, "%d|", &id);
        if (id == del_id) {
            found = 1;
            continue;
        }
        fputs(line, fp_tmp);
    }
    fclose(fp);
    fclose(fp_tmp);
    remove("goods.dat");
    rename("goods.tmp", "goods.dat");
    if (found)
        printf("商品ID %d 删除成功！\n", del_id);
    else
        printf("没有找到该商品ID。\n");
}

// 修改商品
void modify_goods() {
    view_goods();
    printf("请输入要修改的商品ID: ");
    char buf[16];
    if (!fgets(buf, sizeof(buf), stdin)) return;
    int mod_id = atoi(buf);

    FILE *fp = fopen("goods.dat", "r");
    if (!fp) {
        printf("goods.dat不存在！\n");
        return;
    }
    FILE *fp_tmp = fopen("goods.tmp", "w");
    if (!fp_tmp) {
        fclose(fp);
        printf("无法创建临时文件！\n");
        return;
    }
    char line[256];
    int found = 0;
    while (fgets(line, sizeof(line), fp)) {
        int id, category, stock;
        float price;
        char name[64];
        if (sscanf(line, "%d|%63[^|]|%d|%f|%d", &id, name, &category, &price, &stock) == 5) {
            if (id == mod_id) {
                found = 1;
                printf("修改商品ID %d\n", id);
                printf("原名称:%s 新名称(回车保留): ", name);
                char new_name[64];
                if (fgets(new_name, sizeof(new_name), stdin)) {
                    new_name[strcspn(new_name, "\n")] = 0;
                    if (strlen(new_name) > 0) strcpy(name, new_name);
                }
                printf("原类别:%d 新类别(回车保留): ", category);
                char new_cate[8];
                if (fgets(new_cate, sizeof(new_cate), stdin)) {
                    new_cate[strcspn(new_cate, "\n")] = 0;
                    if (strlen(new_cate) > 0) category = atoi(new_cate);
                }
                printf("原价格:%.2f 新价格(回车保留): ", price);
                char new_price[16];
                if (fgets(new_price, sizeof(new_price), stdin)) {
                    new_price[strcspn(new_price, "\n")] = 0;
                    if (strlen(new_price) > 0) price = atof(new_price);
                }
                printf("原库存:%d 新库存(回车保留): ", stock);
                char new_stock[16];
                if (fgets(new_stock, sizeof(new_stock), stdin)) {
                    new_stock[strcspn(new_stock, "\n")] = 0;
                    if (strlen(new_stock) > 0) stock = atoi(new_stock);
                }
                fprintf(fp_tmp, "%d|%s|%d|%.2f|%d\n", id, name, category, price, stock);
            } else {
                fputs(line, fp_tmp);
            }
        }
    }
    fclose(fp);
    fclose(fp_tmp);
    remove("goods.dat");
    rename("goods.tmp", "goods.dat");
    if (found)
        printf("商品ID %d 修改成功！\n", mod_id);
    else
        printf("没有找到该商品ID。\n");
}

// 商品管理菜单循环
void goods_menu_loop(int sockfd) {
    while (1) {
        printf("【商品管理】\n");
        printf("1. 添加商品\n");
        printf("2. 删除商品\n");
        printf("3. 修改商品\n");
        printf("4. 查看商品\n");
        printf("0. 返回上一级\n");
        char input[8];
        printf("商品管理编号: ");
        if (!fgets(input, sizeof(input), stdin)) break;
        input[strcspn(input, "\n")] = 0;
        if (strcmp(input, "0") == 0) break;
        else if (strcmp(input, "1") == 0) add_goods();
        else if (strcmp(input, "2") == 0) delete_goods();
        else if (strcmp(input, "3") == 0) modify_goods();
        else if (strcmp(input, "4") == 0) view_goods();
        else printf("无效输入，请重新输入。\n");
    }
}