// 通信协议头文件（客户端与服务端共享）
// 建议放在 common/ 目录，client 和 server 都 #include "protocol.h"

#ifndef PROTOCOL_H
#define PROTOCOL_H

// 通用常量
#define MAX_NAME_LEN      32
#define MAX_PASS_LEN      32
#define MAX_ADDR_LEN      128
#define MAX_GOODS_NAME    64
#define MAX_SHOP_NAME     64
#define MAX_ORDER_DESC    128
#define MAX_MESSAGE_LEN   256

// 通信命令码（举例，按需扩展）
typedef enum {
    CMD_LOGIN = 1,         // 登录
    CMD_LOGIN_RESULT,      // 登录结果
    CMD_LOGOUT,            // 注销
    CMD_REGISTER,          // 注册
    CMD_REGISTER_RESULT,   // 注册结果
    CMD_QUERY_GOODS,       // 查询商品
    CMD_ADD_GOODS,         // 添加商品
    CMD_DEL_GOODS,         // 删除商品
    CMD_UPDATE_GOODS,      // 修改商品
    CMD_QUERY_ORDER,       // 查询订单
    CMD_ADD_ORDER,         // 下单
    CMD_DEL_ORDER,         // 删除订单
    CMD_UPDATE_ORDER,      // 修改订单
    CMD_QUERY_USER,        // 查询用户
    CMD_ADD_USER,          // 添加用户
    CMD_DEL_USER,          // 删除用户
    CMD_UPDATE_USER,       // 修改用户
    CMD_QUERY_SHOP,        // 查询店铺
    CMD_ADD_SHOP,          // 添加店铺
    CMD_DEL_SHOP,          // 删除店铺
    CMD_UPDATE_SHOP,       // 修改店铺
    CMD_COLLECT_GOODS,     // 收藏商品
    CMD_COLLECT_SHOP,      // 收藏店铺
    CMD_CART_OP,           // 购物车操作
    CMD_SEARCH,            // 搜索
    CMD_COMMENT,           // 评论
    CMD_EXIT,              // 退出
    CMD_RESULT,            // 通用结果
    CMD_ERROR              // 通用错误
    // ... 可继续按需求添加
} CommandType;

// 用户身份
typedef enum {
    USER_IDENTITY_NONE = 0,
    USER_IDENTITY_MASTER,
    USER_IDENTITY_SHOPER,
    USER_IDENTITY_CUSTOMER
} UserIdentity;

// 用户结构体
typedef struct {
    int id;
    char username[MAX_NAME_LEN];
    char password[MAX_PASS_LEN];
    char address[MAX_ADDR_LEN];
    int identity;         // 1=管理员 2=商家 3=顾客
} User;

// 商品结构体
typedef struct {
    int id;
    char name[MAX_GOODS_NAME];
    double price;
    int stock;
    char category[32];
    char shop_name[MAX_SHOP_NAME];
} Goods;

// 订单结构体
typedef struct {
    int id;
    int user_id;
    int goods_id;
    int quantity;
    double total_price;
    char desc[MAX_ORDER_DESC];
    int status;          // 订单状态
} Order;

// 基本消息包结构
typedef struct {
    int cmd;                     // 命令码（CommandType）
    int datalen;                 // 数据长度
    char data[MAX_MESSAGE_LEN];  // 数据区（可根据不同cmd转为不同结构体/字符串）
} Packet;

#endif // PROTOCOL_H