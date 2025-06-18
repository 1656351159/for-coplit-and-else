#include <stdio.h>
#include <string.h>
#include "stat_data.h"
#include "order_data.h"

// 这里假设有一个获取所有订单的函数
// int list_all_orders(Order *buf, int maxn);

double calc_user_total_spent(int user_id) {
    Order buf[1024];
    double total = 0.0;
    int n = list_all_orders(buf, 1024);
    for (int i = 0; i < n; ++i) {
        if (buf[i].user_id == user_id) {
            total += buf[i].total_price;
        }
    }
    return total;
}

// 获取用户的所有历史订单，返回写入buf的订单数量
int get_user_order_history(int user_id, Order *buf, int maxn) {
    Order all_orders[1024];
    int n = list_all_orders(all_orders, 1024);
    int cnt = 0;
    for (int i = 0; i < n && cnt < maxn; ++i) {
        if (all_orders[i].user_id == user_id) {
            buf[cnt++] = all_orders[i];
        }
    }
    return cnt;
}