#ifndef PROTOCOL_H
#define PROTOCOL_H

#define MAX_BUF 4096

// =====================
// 1. 命令类型定义
// =====================
typedef enum {
    // 系统
    CMD_NONE = 0,
    CMD_EXIT,            // 客户端退出
    CMD_RESULT,          // 返回结果
    CMD_ERROR,           // 错误通用

    // 用户相关
    CMD_USER_REGISTER,   // 注册
    CMD_USER_LOGIN,      // 登录
    CMD_USER_LOGOUT,     // 登出
    CMD_USER_MODIFY,     // 修改信息
    CMD_USER_DEL,        // 删除用户
    CMD_USER_VIEW,       // 查看用户列表
    CMD_USER_QUERY,      // 查询某用户

    // 商品相关
    CMD_GOODS_ADD,       // 添加商品
    CMD_GOODS_DEL,       // 删除商品
    CMD_GOODS_MODIFY,    // 修改商品
    CMD_GOODS_VIEW,      // 浏览所有商品
    CMD_GOODS_QUERY,     // 查询商品详细

    // 订单相关
    CMD_ORDER_ADD,       // 下单
    CMD_ORDER_DEL,       // 删除订单
    CMD_ORDER_MODIFY,    // 修改订单
    CMD_ORDER_VIEW,      // 浏览订单
    CMD_ORDER_QUERY,     // 查询某订单

    // 店铺相关
    CMD_STORE_ADD,       // 增加店铺
    CMD_STORE_DEL,       // 删除店铺
    CMD_STORE_MODIFY,    // 修改店铺
    CMD_STORE_VIEW,      // 浏览所有店铺
    CMD_STORE_QUERY,     // 查询某店铺

    // 购物车
    CMD_CART_ADD,        // 添加购物车项
    CMD_CART_DEL,        // 删除购物车项
    CMD_CART_MODIFY,     // 修改购物车项
    CMD_CART_VIEW,       // 查看购物车

    // 收藏
    CMD_FAV_ADD,         // 添加收藏
    CMD_FAV_DEL,         // 删除收藏
    CMD_FAV_VIEW,        // 查看收藏

    // 其它
    CMD_STAT_TOTAL,      // 统计消费金额
    CMD_STAT_HISTORY     // 消费历史

    // 可拓展
} CmdType;


// =====================
// 2. 基本数据结构
// =====================

typedef struct {
    int id;
    char username[32];
    char password[32];
    char phone[32];
    char email[64];
    int role; // 0:普通用户 1:管理员 2:商家
} User;

typedef struct {
    int id;
    char name[64];
    char category[32]; // 如"饮品"、"快餐"等
    double price;
    int stock;
    int store_id; // 所属店铺ID
} Goods;

typedef struct {
    int id;
    char name[64];
    char address[128];
    char phone[32];
    int owner_id; // 用户id
} Store;

typedef struct {
    int id;
    int user_id;           // 下单用户
    int goods_id;          // 商品
    int quantity;
    double total_price;
    int status;            // 订单状态（0未支付，1已支付，2已完成，3已取消等）
    char created_at[32];   // 下单时间
} Order;

typedef struct {
    int id;
    int user_id;
    int goods_id;
    int quantity;
} CartItem;

typedef struct {
    int id;
    int user_id;
    int goods_id;
} Favorite;

// =====================
// 3. 协议包定义
// =====================

typedef struct {
    int cmd;                // CmdType，命令号
    char data[MAX_BUF];     // 存储实际内容（如结构体或文本）
} ProtocolPacket;

#endif // PROTOCOL_H