// 通信协议头文件（与客户端共享）
// 本文件用于定义客户端与服务端之间通信的命令、结构体与数据包格式

#ifndef PROTOCOL_H
#define PROTOCOL_H

// 最大长度定义
#define MAX_NAME_LEN      32
#define MAX_PASS_LEN      32
#define MAX_CATEGORY_LEN  32
#define MAX_DESC_LEN      128
#define MAX_GOODS_NAME    64
#define MAX_SHOP_NAME     64
#define MAX_ADDR_LEN      128
#define MAX_BUF           256

// 命令类型（按需补充）
typedef enum {
    CMD_LOGIN = 1,         // 登录
    CMD_LOGIN_RESULT,      // 登录结果
    CMD_REGISTER,          // 注册
    CMD_REGISTER_RESULT,   // 注册结果
    CMD_GOODS_ADD,         // 添加商品
    CMD_GOODS_DEL,         // 删除商品
    CMD_GOODS_MODIFY,      // 修改商品
    CMD_GOODS_VIEW,        // 浏览商品
    CMD_USER_ADD,          // 添加用户
    CMD_USER_DEL,          // 删除用户
    CMD_USER_MODIFY,       // 修改用户
    CMD_USER_VIEW,         // 浏览用户
    CMD_SHOP_ADD,          // 添加店铺
    CMD_SHOP_DEL,          // 删除店铺
    CMD_SHOP_MODIFY,       // 修改店铺
    CMD_SHOP_VIEW,         // 浏览店铺
    CMD_ORDER_ADD,         // 添加订单
    CMD_ORDER_DEL,         // 删除订单
    CMD_ORDER_MODIFY,      // 修改订单
    CMD_ORDER_VIEW,        // 浏览订单
    CMD_EXIT,              // 退出
    CMD_RESULT,            // 通用结果
    CMD_ERROR              // 通用错误
} CmdType;

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
    int identity; // 用户身份
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

// 基本消息包结构
typedef struct {
    int cmd;                  // 命令类型（CmdType）
    char data[MAX_BUF];       // 数据区（用于发送结构体/字符串等）
} ProtocolPacket;

#endif // PROTOCOL_H