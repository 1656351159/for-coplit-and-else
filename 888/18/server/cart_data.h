#ifndef CART_DATA_H
#define CART_DATA_H

#include "../common/protocol.h"

// 添加购物车项，返回新id，失败返回-1
int add_cart(const CartItem* c);
// 删除购物车项（按id），成功返回0，失败-1
int delete_cart(int cart_id);
// 修改购物车项，成功返回0，失败-1
int update_cart(const CartItem* c);
// 查询购物车项（按id），成功返回0，失败-1
int get_cart_by_id(int cart_id, CartItem* out);
// 查询某用户购物车所有项，返回数量
int list_cart_by_user(int user_id, CartItem* buf, int max_count);

#endif