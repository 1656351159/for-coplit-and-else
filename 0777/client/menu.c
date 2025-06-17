#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "menu.h"
#include "client_util.h"
#include "goods_menu.h"
#include "favorite_menu.h"
#include "category_menu.h"
#include "order_menu.h"
#include "profile_menu.h"
#include "cart_menu.h"

// 只声明，不实现
void category_menu_loop(int sockfd);
void goods_menu_loop(int sockfd);
void store_collect_menu_loop(int sockfd);
void user_menu_loop(int sockfd);
void search_menu_loop(int sockfd);

int current_user_id = 0;

// 管理员菜单显示
void show_master_menu() {
    printf("【管理员菜单】\n");
    printf("1. 首页\n");
    printf("2. 商品管理\n");
    printf("3. 店铺管理\n");
    printf("4. 店铺收藏\n");
    printf("5. 用户管理\n");
    printf("6. 订单管理\n");
    printf("7. 个人中心\n");
    printf("0. 退出系统\n");
}

// 商家菜单显示
void show_shoper_menu() {
    printf("【商家菜单】\n");
    printf("1. 首页\n");
    printf("2. 商品管理\n");
    printf("3. 订单管理\n");
    printf("4. 个人中心\n");
    printf("0. 退出系统\n");
}

// 顾客菜单显示
void show_customer_menu() {
    printf("【顾客菜单】\n");
    printf("1. 首页\n");
    printf("2. 商品收藏\n");
    printf("3. 购物车\n");
    printf("4. 订单管理\n");
    printf("5. 个人中心\n");
    printf("0. 退出系统\n");
}

// 首页菜单显示
void show_home_menu() {
    printf("【首页】\n");
    printf("1. 商品分类\n");
    printf("2. 商品收藏\n");
    printf("3. 搜索商品\n");
    printf("0. 返回上一级\n");
}

// 商品分类菜单显示
void show_category_menu() {
    printf("【商品分类】\n");
    printf("1. 中餐\n");
    printf("2. 西餐\n");
    printf("3. 快餐\n");
    printf("4. 饮品\n");
    printf("5. 拼好饭\n");
    printf("0. 返回上一级\n");
}

// 管理员主菜单循环
void master_loop(int sockfd) {
    while (1) {
        show_master_menu();
        char input[8];
        input_str("请输入编号: ", input, sizeof(input));
        if (strcmp(input, "0") == 0) break;
        else if (strcmp(input, "1") == 0) home_menu_loop(sockfd);
        else if (strcmp(input, "2") == 0) goods_menu_loop(sockfd);
        else if (strcmp(input, "3") == 0) store_menu_loop(sockfd);
        else if (strcmp(input, "4") == 0) store_collect_menu_loop(sockfd);
        else if (strcmp(input, "5") == 0) user_menu_loop(sockfd);
        else if (strcmp(input, "6") == 0) order_menu_loop(sockfd);
        else if (strcmp(input, "7") == 0) profile_menu_loop(sockfd);
    }
}

// 商家主菜单循环
void shoper_loop(int sockfd) {
    while (1) {
        show_shoper_menu();
        char input[8];
        input_str("请输入编号: ", input, sizeof(input));
        if (strcmp(input, "0") == 0) break;
        else if (strcmp(input, "1") == 0) home_menu_loop(sockfd);
        else if (strcmp(input, "2") == 0) goods_menu_loop(sockfd);
        else if (strcmp(input, "3") == 0) order_menu_loop(sockfd);
        else if (strcmp(input, "4") == 0) profile_menu_loop(sockfd);
    }
}

// 顾客主菜单循环
void customer_loop(int sockfd) {
    while (1) {
        show_customer_menu();
        char input[8];
        input_str("请输入编号: ", input, sizeof(input));
        if (strcmp(input, "0") == 0) break;
        else if (strcmp(input, "1") == 0) home_menu_loop(sockfd);
        else if (strcmp(input, "2") == 0) favorite_menu_loop(sockfd);
        else if (strcmp(input, "3") == 0) cart_menu_loop(sockfd);
        else if (strcmp(input, "4") == 0) order_menu_loop(sockfd);
        else if (strcmp(input, "5") == 0) profile_menu_loop(sockfd);
    }
}

// 首页菜单循环
void home_menu_loop(int sockfd) {
    while (1) {
        show_home_menu();
        char input[8];
        input_str("首页编号: ", input, sizeof(input));
        if (strcmp(input, "0") == 0) break;
        else if (strcmp(input, "1") == 0) category_menu_loop(sockfd);
        else if (strcmp(input, "2") == 0) favorite_menu_loop(sockfd);
        else if (strcmp(input, "3") == 0) search_menu_loop(sockfd);
    }
}

// 获取下一个店铺ID
int get_next_store_id() {
    FILE *fp = fopen("store.dat", "r");
    if (!fp) return 1;
    int last_id = 0;
    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%d|", &last_id);
    }
    fclose(fp);
    return last_id + 1;
}

// 添加店铺
void add_store() {
    char name[64], address[128], phone[32];
    printf("请输入店铺名称: ");
    if (!fgets(name, sizeof(name), stdin)) return;
    name[strcspn(name, "\n")] = 0;
    printf("请输入店铺地址: ");
    if (!fgets(address, sizeof(address), stdin)) return;
    address[strcspn(address, "\n")] = 0;
    printf("请输入联系电话: ");
    if (!fgets(phone, sizeof(phone), stdin)) return;
    phone[strcspn(phone, "\n")] = 0;

    int id = get_next_store_id();
    FILE *fp = fopen("store.dat", "a");
    if (!fp) {
        printf("无法打开店铺数据文件！\n");
        return;
    }
    fprintf(fp, "%d|%s|%s|%s\n", id, name, address, phone);
    fclose(fp);
    printf("店铺添加成功！店铺ID为 %d\n", id);
}

// 查看所有店铺
void view_stores() {
    FILE *fp = fopen("store.dat", "r");
    if (!fp) {
        printf("暂无店铺信息。\n");
        return;
    }
    printf("店铺列表：\nID\t名称\t\t地址\t\t\t电话\n");
    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        int id;
        char name[64], address[128], phone[32];
        if (sscanf(line, "%d|%63[^|]|%127[^|]|%31s", &id, name, address, phone) == 4) {
            printf("%d\t%s\t%s\t%s\n", id, name, address, phone);
        }
    }
    fclose(fp);
}

// 删除店铺
void delete_store() {
    view_stores();
    printf("请输入要删除的店铺ID: ");
    char buf[16];
    if (!fgets(buf, sizeof(buf), stdin)) return;
    int del_id = atoi(buf);
    FILE *fp = fopen("store.dat", "r");
    if (!fp) {
        printf("店铺数据文件不存在！\n");
        return;
    }
    FILE *fp_tmp = fopen("store.tmp", "w");
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
            continue; // 跳过要删除的那一行
        }
        fputs(line, fp_tmp);
    }
    fclose(fp);
    fclose(fp_tmp);
    remove("store.dat");
    rename("store.tmp", "store.dat");
    if (found)
        printf("店铺ID %d 删除成功！\n", del_id);
    else
        printf("没有找到该店铺ID。\n");
}

// 修改店铺
void modify_store() {
    view_stores();
    printf("请输入要修改的店铺ID: ");
    char buf[16];
    if (!fgets(buf, sizeof(buf), stdin)) return;
    int mod_id = atoi(buf);
    FILE *fp = fopen("store.dat", "r");
    if (!fp) {
        printf("店铺数据文件不存在！\n");
        return;
    }
    FILE *fp_tmp = fopen("store.tmp", "w");
    if (!fp_tmp) {
        fclose(fp);
        printf("无法创建临时文件！\n");
        return;
    }
    char line[256];
    int found = 0;
    while (fgets(line, sizeof(line), fp)) {
        int id;
        char name[64], address[128], phone[32];
        if (sscanf(line, "%d|%63[^|]|%127[^|]|%31s", &id, name, address, phone) == 4) {
            if (id == mod_id) {
                found = 1;
                printf("正在修改店铺ID %d\n", id);
                printf("原名称: %s\n新名称(直接回车保留): ", name);
                char new_name[64];
                if (fgets(new_name, sizeof(new_name), stdin)) {
                    new_name[strcspn(new_name, "\n")] = 0;
                    if (strlen(new_name) > 0) strcpy(name, new_name);
                }
                printf("原地址: %s\n新地址(直接回车保留): ", address);
                char new_address[128];
                if (fgets(new_address, sizeof(new_address), stdin)) {
                    new_address[strcspn(new_address, "\n")] = 0;
                    if (strlen(new_address) > 0) strcpy(address, new_address);
                }
                printf("原电话: %s\n新电话(直接回车保留): ", phone);
                char new_phone[32];
                if (fgets(new_phone, sizeof(new_phone), stdin)) {
                    new_phone[strcspn(new_phone, "\n")] = 0;
                    if (strlen(new_phone) > 0) strcpy(phone, new_phone);
                }
                fprintf(fp_tmp, "%d|%s|%s|%s\n", id, name, address, phone);
            } else {
                fputs(line, fp_tmp);
            }
        }
    }
    fclose(fp);
    fclose(fp_tmp);
    remove("store.dat");
    rename("store.tmp", "store.dat");
    if (found)
        printf("店铺ID %d 修改成功！\n", mod_id);
    else
        printf("没有找到该店铺ID。\n");
}

// 店铺管理菜单
void store_menu_loop(int sockfd) {
    while (1) {
        printf("【店铺管理】\n");
        printf("1. 添加店铺\n");
        printf("2. 删除店铺\n");
        printf("3. 修改店铺\n");
        printf("4. 查看店铺\n");
        printf("0. 返回上一级\n");
        char input[8];
        printf("店铺管理编号: ");
        if (!fgets(input, sizeof(input), stdin)) break;
        input[strcspn(input, "\n")] = 0;
        if (strcmp(input, "0") == 0) break;
        else if (strcmp(input, "1") == 0) {
            add_store();
        } else if (strcmp(input, "2") == 0) {
            delete_store();
        } else if (strcmp(input, "3") == 0) {
            modify_store();
        } else if (strcmp(input, "4") == 0) {
            view_stores();
        } else {
            printf("无效输入，请重新输入。\n");
        }
    }
}