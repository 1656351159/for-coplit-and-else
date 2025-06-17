#ifndef PROTOCOL_H
#define PROTOCOL_H

#define MAX_BUF 1024
#define MAX_NAME 64

// 命令类型
typedef enum {
    CMD_MENU_SELECT,       // 菜单选择
    CMD_EXIT,              // 退出
    CMD_GOODS_ADD,         // 添加商品
    CMD_GOODS_DEL,         // 删除商品
    CMD_GOODS_MODIFY,      // 修改商品
    CMD_GOODS_VIEW,        // 查看商品
    CMD_STORE_ADD,         // 添加店铺
    CMD_STORE_DEL,         // 删除店铺
    CMD_STORE_MODIFY,      // 修改店铺
    CMD_STORE_VIEW,        // 查看店铺
    CMD_STORE_COLLECT_VIEW,// 查看收藏的店铺
    CMD_STORE_COLLECT_DEL, // 删除收藏的店铺
    CMD_USER_VIEW,         // 查看用户
    CMD_USER_DEL,          // 删除用户
    CMD_USER_MODIFY,       // 修改用户信息
    CMD_ORDER_VIEW,        // 查看订单
    CMD_ORDER_DEL,         // 删除订单
    CMD_ORDER_MODIFY,      // 修改订单状态
    CMD_PROFILE_MODIFY,    // 修改个人信息
    CMD_PROFILE_VIEW,      // 查看个人信息
    CMD_MODIFY_PASSWORD,   // 修改密码
    CMD_COMMENT,           // 评论
    CMD_CART_VIEW,         // 查看购物车
    CMD_CART_DEL,          // 删除购物车商品
    CMD_CART_MODIFY,       // 修改购物车商品数量
    CMD_FAVORITE_VIEW,     // 查看商品收藏
    CMD_FAVORITE_DEL,      // 删除商品收藏
    CMD_SEARCH,            // 搜索商品
    CMD_HISTORY,           // 查看搜索历史
    CMD_CUSTOM,            // 备用自定义命令
} CmdType;

// 商品结构体
typedef struct {
    int id;
    char name[MAX_NAME];
    char category[MAX_NAME]; // 分类
    double price;
    int stock;
    char description[128];
} Goods;

// 店铺结构体
typedef struct {
    int id;
    char name[MAX_NAME];
    char owner[MAX_NAME];
    char description[128];
} Store;

// 用户结构体
typedef struct {
    int id;
    char username[MAX_NAME];
    char password[MAX_NAME];
    char address[128];
    char role[16]; // master/shoper/customer
} User;

// 订单结构体
typedef struct {
    int id;
    int user_id;
    int goods_id;
    int store_id;
    int count;
    char status[32]; // 已下单/已支付/已完成等
    char comment[128];
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
    int count;
} Cart;

// 通信包
typedef struct {
    CmdType cmd;
    char data[MAX_BUF]; // 存储用户输入、结构体序列化、查询参数等
} ProtocolPacket;

#endif