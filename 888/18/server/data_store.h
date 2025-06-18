// data_store.h 数据存储操作头文件
#ifndef DATA_STORE_H
#define DATA_STORE_H

#include "../common/protocol.h"

// 商品相关
int add_goods(const Goods* g);
int delete_goods(int goods_id);
int update_goods(const Goods* g);
int get_goods_by_id(int goods_id, Goods* out);
int list_all_goods(Goods* buf, int max_count); // 返回实际数量

// 用户相关
int add_user(const User* u);
int delete_user(int user_id);
int update_user(const User* u);
int get_user_by_id(int user_id, User* out);
int list_all_users(User* buf, int max_count);

// 店铺和订单相关接口可按需扩展...

#endif // DATA_STORE_H