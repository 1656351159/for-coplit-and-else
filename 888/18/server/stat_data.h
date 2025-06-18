#ifndef STAT_DATA_H
#define STAT_DATA_H

#include "order_data.h"  // 这一句非常关键！

double calc_user_total_spent(int user_id);
int get_user_order_history(int user_id, Order *buf, int maxn);

#endif