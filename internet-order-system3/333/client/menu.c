// 客户端菜单相关实现

#include <stdio.h>
#include <string.h>
#include "menu.h"

// 输入辅助函数
void input_str(const char* prompt, char* buf, int size) {
    printf("%s", prompt);
    if (fgets(buf, size, stdin)) {
        buf[strcspn(buf, "\n")] = 0;
    }
}

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

// 其它菜单显示函数省略，可按你的需求继续补充...

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

// 商品分类菜单循环
void category_menu_loop(int sockfd) {
    while (1) {
        show_category_menu();
        char input[8];
        input_str("商品分类编号: ", input, sizeof(input));
        if (strcmp(input, "0") == 0) break;
        // 这里可继续完善各类商品菜单
        else printf("功能开发中...\n");
    }
}

// 其它菜单循环函数省略，请根据你的需求自行补充...

// 示例：商品管理菜单循环
void goods_menu_loop(int sockfd) {
    while (1) {
        printf("【商品管理】\n");
        printf("1. 添加商品\n");
        printf("2. 删除商品\n");
        printf("3. 修改商品\n");
        printf("4. 查看商品\n");
        printf("0. 返回上一级\n");
        char input[8];
        input_str("商品管理编号: ", input, sizeof(input));
        if (strcmp(input, "0") == 0) break;
        else printf("功能开发中...\n");
    }
}

// 其它功能可继续照此补充完善...