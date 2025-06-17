#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "category_menu.h"
// 显示指定分类下的商品
void show_goods_in_category(int category) {
    FILE *fp = fopen("goods.dat", "r");
    if (!fp) {
        printf("暂无商品信息。\n");
        return;
    }
    printf("ID\t名称\t价格\t库存\n");
    char line[256];
    int found = 0;
    while (fgets(line, sizeof(line), fp)) {
        int id, cate, stock;
        float price;
        char name[64];
        if (sscanf(line, "%d|%63[^|]|%d|%f|%d", &id, name, &cate, &price, &stock) == 5) {
            if (cate == category) {
                printf("%d\t%s\t%.2f\t%d\n", id, name, price, stock);
                found = 1;
            }
        }
    }
    fclose(fp);
    if (!found) printf("暂无此分类下商品。\n");
}

// 分类菜单循环（带操作）
void category_menu_loop(int sockfd) {
    while (1) {
        printf("【商品分类】\n");
        printf("1. 中餐\n2. 西餐\n3. 快餐\n4. 饮品\n5. 拼好饭\n0. 返回上一级\n");
        char input[8];
        printf("商品分类编号: ");
        if (!fgets(input, sizeof(input), stdin)) break;
        input[strcspn(input, "\n")] = 0;
        int category = atoi(input);
        if (category == 0) break;
        if (category < 1 || category > 5) {
            printf("无效输入，请重新输入。\n");
            continue;
        }
        show_goods_in_category(category);

        // 商品操作
        while (1) {
            printf("输入商品ID进行操作，或0返回分类菜单：");
            char idbuf[16];
            if (!fgets(idbuf, sizeof(idbuf), stdin)) return;
            int gid = atoi(idbuf);
            if (gid == 0) break;
            printf("1. 加入购物车  2. 收藏商品  0. 返回上一级\n");
            printf("请选择操作: ");
            char opbuf[8];
            if (!fgets(opbuf, sizeof(opbuf), stdin)) break;
            opbuf[strcspn(opbuf, "\n")] = 0;
            if (strcmp(opbuf, "0") == 0) break;
            else if (strcmp(opbuf, "1") == 0) {
                printf("加入购物车功能开发中...(可以在这里调用cart_add函数)\n");
            } else if (strcmp(opbuf, "2") == 0) {
                printf("收藏商品功能开发中...(可以在这里调用favorite_add函数)\n");
            } else {
                printf("无效操作。\n");
            }
        }
    }
}