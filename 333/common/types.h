// types.h 各种结构体定义（供client/server都包含）

#ifndef TYPES_H
#define TYPES_H

// 最大长度定义
#define MAX_NAME_LEN      32
#define MAX_PASS_LEN      32
#define MAX_CATEGORY_LEN  32
#define MAX_DESC_LEN      128
#define MAX_GOODS_NAME    64
#define MAX_SHOP_NAME     64
#define MAX_ADDR_LEN      128

// 用户身份
typedef enum {
    USER_IDENTITY_NONE = 0,
    USER_IDENTITY_MASTER = 1,   // 管理员
    USER_IDENTITY_SHOPER = 2,   // 商家
    USER_IDENTITY_CUSTOMER = 3  // 顾客
} UserIdentity;

// 用户结构体
typedef struct {
    int id;
    char username[MAX_NAME_LEN];
    char password[MAX_PASS_LEN];
    char address[MAX_ADDR_LEN];
    int identity; // 用户身份 UserIdentity
} User;

// 商品结构体
typedef struct {
    int id;
    char name[MAX_GOODS_NAME];
    char category[MAX_CATEGORY_LEN];
    double price;
    int stock;
    char description[MAX_DESC_LEN];
} Goods;

// 店铺结构体
typedef struct {
    int id;
    char name[MAX_SHOP_NAME];
    char owner[MAX_NAME_LEN];
    char description[MAX_DESC_LEN];
} Shop;

// 订单结构体
typedef struct {
    int id;
    int user_id;
    int goods_id;
    int quantity;
    double total_price;
    char desc[MAX_DESC_LEN];
    int status;
} Order;

#endif // TYPES_H