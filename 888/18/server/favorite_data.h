#ifndef FAVORITE_DATA_H
#define FAVORITE_DATA_H

#include "../common/protocol.h"

// 添加收藏，返回新id，失败返回-1
int add_favorite(const Favorite* f);
// 删除收藏（按id），成功返回0，失败-1
int delete_favorite(int fav_id);
// 查找用户所有收藏，返回数量
int list_favorites_by_user(int user_id, Favorite* buf, int max_count);

#endif