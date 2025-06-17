#ifndef STORE_DATA_H
#define STORE_DATA_H

#include "../common/protocol.h"

// 添加店铺，返回新id，失败返回-1
int add_store(const Store* s);
// 删除店铺（按id），成功返回0，失败-1
int delete_store(int store_id);
// 修改店铺，成功返回0，失败-1
int update_store(const Store* s);
// 按id查找店铺，成功返回0，失败-1
int get_store_by_id(int store_id, Store* out);
// 获取全部店铺，返回数量
int list_all_stores(Store* buf, int max_count);

#endif