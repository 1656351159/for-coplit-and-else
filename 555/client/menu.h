// 客户端菜单头文件，声明所有菜单相关函数
#ifndef MENU_H
#define MENU_H

// 管理员菜单主循环
void master_loop(int sockfd);

// 商家菜单主循环
void shoper_loop(int sockfd);

// 顾客菜单主循环
void customer_loop(int sockfd);

// 首页菜单主循环
void home_menu_loop(int sockfd);

// 商品分类菜单主循环
void category_menu_loop(int sockfd);

// 商品管理菜单主循环
void goods_menu_loop(int sockfd);

// 店铺管理菜单主循环
void store_menu_loop(int sockfd);

// 店铺收藏菜单主循环
void store_collect_menu_loop(int sockfd);

// 用户管理菜单主循环
void user_menu_loop(int sockfd);

// 订单管理菜单主循环
void order_menu_loop(int sockfd);

// 个人中心菜单主循环
void profile_menu_loop(int sockfd);

// 收藏菜单主循环（顾客用）
void favorite_menu_loop(int sockfd);

// 购物车菜单主循环（顾客用）
void cart_menu_loop(int sockfd);

// 搜索菜单主循环
void search_menu_loop(int sockfd);

// 各级菜单显示函数（如需在其它文件直接调用可声明，否则可在menu.c内static实现）
void show_master_menu();
void show_shoper_menu();
void show_customer_menu();
void show_home_menu();
void show_category_menu();

void add_store();
void delete_store();
void modify_store();
void view_stores();

// 输入辅助函数
void input_str(const char* prompt, char* buf, int size);

#endif // MENU_H