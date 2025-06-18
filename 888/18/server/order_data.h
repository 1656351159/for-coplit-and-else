#ifndef ORDER_DATA_H
#define ORDER_DATA_H

#include "../common/protocol.h"



// 新增订单，返回新订单id，失败返回-1
int add_order(const Order* o);
// 删除订单，成功返回0，失败-1
int delete_order(int order_id);
// 修改订单，成功返回0，失败-1
int update_order(const Order* o);
// 按ID查找订单，成功返回0，失败-1
int get_order_by_id(int order_id, Order* out);
// 获取所有订单，返回订单数量
int list_all_orders(Order* buf, int max_count);

#endif