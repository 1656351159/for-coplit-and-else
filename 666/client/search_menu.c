#include "search_menu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// 忽略大小写字符串包含
int strcasestr_cn(const char* a, const char* b) {
    char str_a[128], str_b[128];
    strncpy(str_a, a, sizeof(str_a) - 1);
    strncpy(str_b, b, sizeof(str_b) - 1);
    str_a[sizeof(str_a) - 1] = '\0';
    str_b[sizeof(str_b) - 1] = '\0';
    for (char *p = str_a; *p; ++p) *p = tolower(*p);
    for (char *p = str_b; *p; ++p) *p = tolower(*p);
    return strstr(str_a, str_b) != NULL;
}

// 按商品名称模糊查找
void search_goods_by_keyword(const char *keyword) {
    FILE *fp = fopen("goods.dat", "r");
    if (!fp) {
        printf("无法打开商品数据文件。\n");
        return;
    }
    char line[256];
    int found = 0;
    printf("搜索结果（商品名称包含: %s）:\n", keyword);
    printf("商品ID\t名称\t类别\t价格\t库存\n");
    while (fgets(line, sizeof(line), fp)) {
        int id, category, stock;
        float price;
        char name[64];
        if (sscanf(line, "%d|%63[^|]|%d|%f|%d", &id, name, &category, &price, &stock) == 5) {
            if (strcasestr_cn(name, keyword)) {
                printf("%d\t%s\t%d\t%.2f\t%d\n", id, name, category, price, stock);
                found = 1;
            }
        }
    }
    fclose(fp);
    if (!found) {
        printf("没有找到包含该关键字的商品。\n");
    }
}

// 按商品正价模糊查找
void fuzzy_search_by_price() {
    char price_str[32];
    printf("请输入要搜索的商品正价（可输入部分数字，如'15'）：");
    if (!fgets(price_str, sizeof(price_str), stdin)) return;
    price_str[strcspn(price_str, "\n")] = 0;
    if (strlen(price_str) == 0) {
        printf("输入不能为空！\n");
        return;
    }
    FILE *fp = fopen("goods.dat", "r");
    if (!fp) {
        printf("无法打开商品数据文件。\n");
        return;
    }
    printf("搜索结果（正价包含关键字: %s）:\n", price_str);
    printf("商品ID\t名称\t类别\t价格\t库存\n");
    char line[256];
    int found = 0;
    while (fgets(line, sizeof(line), fp)) {
        int id, category, stock;
        float price;
        char name[64];
        if (sscanf(line, "%d|%63[^|]|%d|%f|%d", &id, name, &category, &price, &stock) == 5) {
            char price_buf[32];
            snprintf(price_buf, sizeof(price_buf), "%.2f", price);
            if (strcasestr_cn(price_buf, price_str)) {
                printf("%d\t%s\t%d\t%.2f\t%d\n", id, name, category, price, stock);
                found = 1;
            }
        }
    }
    fclose(fp);
    if (!found) {
        printf("没有找到正价包含该关键字的商品。\n");
    }
}

void search_menu_loop(int sockfd) {
    while (1) {
        printf("【商品搜索】\n");
        printf("1. 关键字查找商品\n");
        printf("2. 正价模糊搜索\n");
        printf("0. 返回上一级\n");
        char input[8];
        printf("菜单编号: ");
        if (!fgets(input, sizeof(input), stdin)) break;
        input[strcspn(input, "\n")] = 0;
        if (strcmp(input, "0") == 0) break;
        else if (strcmp(input, "1") == 0) {
            char keyword[64];
            printf("请输入要查找的商品关键字: ");
            if (fgets(keyword, sizeof(keyword), stdin)) {
                keyword[strcspn(keyword, "\n")] = 0;
                if (strlen(keyword) > 0)
                    search_goods_by_keyword(keyword);
                else
                    printf("关键字不能为空！\n");
            }
        } else if (strcmp(input, "2") == 0) {
            fuzzy_search_by_price();
        } else {
            printf("无效输入，请重新输入。\n");
        }
    }
}