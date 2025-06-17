#ifndef USER_DATA_H
#define USER_DATA_H

#include "../common/protocol.h"

// 注册用户，返回新用户id，失败返回-1
int add_user(const User* u);
// 删除用户，成功返回0，失败-1
int delete_user(int user_id);
// 修改用户信息，成功返回0，失败-1
int update_user(const User* u);
// 按ID查找用户，成功返回0，失败-1
int get_user_by_id(int user_id, User* out);
// 按用户名查找用户，成功返回0，失败-1
int get_user_by_name(const char* username, User* out);
// 获取所有用户，返回用户数量
int list_all_users(User* buf, int max_count);

#endif