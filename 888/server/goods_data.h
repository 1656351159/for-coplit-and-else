#ifndef GOODS_DATA_H
#define GOODS_DATA_H

#include "../common/protocol.h"

// 返回新商品id，失败返回-1
int add_goods(const Goods* g);
// 删除商品，成功返回0，失败-1
int delete_goods(int goods_id);
// 修改商品，成功返回0，失败-1
int update_goods(const Goods* g);
// 按ID查找，成功返回0，失败-1
int get_goods_by_id(int goods_id, Goods* out);
// 读取所有商品，返回商品数量
int list_all_goods(Goods* buf, int max_count);

#endif