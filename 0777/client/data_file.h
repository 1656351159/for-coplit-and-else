#ifndef DATA_FILE_H
#define DATA_FILE_H

// 用户结构体
typedef struct {
    int id;
    char username[32];
    char phone[32];
    char email[64];
} User;

// 商品结构体
typedef struct {
    int id;
    char name[64];
    int category;
    float price;
    int stock;
} Goods;

// 订单结构体
typedef struct {
    int order_id;
    int user_id;
    int goods_id;
    int quantity;
    char status[32];
} Order;

// 收藏结构体
typedef struct {
    int user_id;
    int goods_id;
} Favorite;

// 购物车结构体
typedef struct {
    int user_id;
    int goods_id;
    int quantity;
} Cart;

// 店铺结构体（如已实现，可自行扩展）
typedef struct {
    int store_id;
    char name[64];
    char address[128];
} Store;

// 数据读写函数声明
int read_users(User *arr, int max_count);
int write_users(const User *arr, int count);

int read_goods(Goods *arr, int max_count);
int write_goods(const Goods *arr, int count);

int read_orders(Order *arr, int max_count);
int write_orders(const Order *arr, int count);

int read_favorites(Favorite *arr, int max_count);
int write_favorites(const Favorite *arr, int count);

int read_cart(Cart *arr, int max_count);
int write_cart(const Cart *arr, int count);

int read_stores(Store *arr, int max_count);
int write_stores(const Store *arr, int count);

#endif // DATA_FILE_H