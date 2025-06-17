#ifndef MENU_H
#define MENU_H

void show_master_menu();
void show_shoper_menu();
void show_customer_menu();
void show_home_menu();
void show_category_menu();

void master_loop(int sockfd);
void shoper_loop(int sockfd);
void customer_loop(int sockfd);
void home_menu_loop(int sockfd);
void store_menu_loop(int sockfd);
void category_menu_loop(int sockfd);
void goods_menu_loop(int sockfd);
void store_collect_menu_loop(int sockfd);
void user_menu_loop(int sockfd);
void search_menu_loop(int sockfd);
void cart_menu_loop(int sockfd);
void favorite_menu_loop(int sockfd);
void order_menu_loop(int sockfd);
void profile_menu_loop(int sockfd);

#endif // MENU_H