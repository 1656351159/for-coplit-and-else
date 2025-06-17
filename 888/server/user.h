#ifndef USER_H
#define USER_H

// role: 1=管理员 2=商家 3=用户
void user_register(int sockfd, int role);
void user_login(int sockfd, int role);
void user_forgot_pwd(int sockfd, const char *name, int role);

#endif#ifndef USER_H
#define USER_H

// role: 1=管理员 2=商家 3=用户
void user_register(int sockfd, int role);
void user_login(int sockfd, int role);
void user_forgot_pwd(int sockfd, const char *name, int role);

#endif