#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "../common/protocol.h"
#include "handler.h"
#include "user_data.h"
#include "goods_data.h"
#include "order_data.h"
#include "cart_data.h"
#include "favorite_data.h"
#include "store_data.h"
#include "stat_data.h"

#define MAX_REPLY_BUF 4096

// 工具函数：发送字符串回复
static void send_str(int sockfd, int cmd, const char* str) {
    ProtocolPacket p = {0};
    p.cmd = cmd;
    strncpy(p.data, str, MAX_REPLY_BUF-1);
    p.data[MAX_REPLY_BUF-1] = '\0';
    send(sockfd, &p, sizeof(p), 0);
}

void* handle_client(void* arg) {
    struct client_info* cinfo = (struct client_info*)arg;
    int client_sockfd = cinfo->client_sockfd;
    struct sockaddr_in client_addr = cinfo->client_addr;
    free(cinfo);

    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));
    int client_port = ntohs(client_addr.sin_port);

    printf("新连接已建立，fd=%d，来自 %s:%d\n", client_sockfd, client_ip, client_port);

    while (1) {
        ProtocolPacket packet = {0};
        ssize_t recv_len = recv(client_sockfd, &packet, sizeof(packet), 0);
        if (recv_len > 0) {
            switch (packet.cmd) {
                // ======= 系统命令 =======
                case CMD_EXIT:
                    send_str(client_sockfd, CMD_EXIT, "服务器已处理退出请求。");
                    goto done;

                // ======= 用户相关 =======
                case CMD_USER_REGISTER: {
                    User u;
                    memcpy(&u, packet.data, sizeof(User));
                    User tmp;
                    if (get_user_by_name(u.username, &tmp) == 0) {
                        send_str(client_sockfd, CMD_USER_REGISTER, "用户名已存在");
                    } else {
                        int new_id = add_user(&u);
                        if (new_id > 0) send_str(client_sockfd, CMD_USER_REGISTER, "注册成功");
                        else send_str(client_sockfd, CMD_USER_REGISTER, "注册失败");
                    }
                    break;
                }
                case CMD_USER_LOGIN: {
                    User req_u;
                    memcpy(&req_u, packet.data, sizeof(User));
                    User found_u;
                    if (get_user_by_name(req_u.username, &found_u) == 0 && strcmp(req_u.password, found_u.password) == 0) {
                        send_str(client_sockfd, CMD_USER_LOGIN, "登录成功");
                    } else {
                        send_str(client_sockfd, CMD_USER_LOGIN, "用户名或密码错误");
                    }
                    break;
                }
                case CMD_USER_MODIFY: {
                    User u;
                    memcpy(&u, packet.data, sizeof(User));
                    int ret = update_user(&u);
                    if (ret == 0) send_str(client_sockfd, CMD_USER_MODIFY, "修改成功");
                    else send_str(client_sockfd, CMD_USER_MODIFY, "修改失败");
                    break;
                }
                case CMD_USER_DEL: {
                    int user_id;
                    memcpy(&user_id, packet.data, sizeof(int));
                    int ret = delete_user(user_id);
                    if (ret == 0) send_str(client_sockfd, CMD_USER_DEL, "删除成功");
                    else send_str(client_sockfd, CMD_USER_DEL, "删除失败");
                    break;
                }
                case CMD_USER_VIEW: {
                    User buf[1024];
                    int n = list_all_users(buf, 1024);
                    char out[MAX_REPLY_BUF] = "";
                    for (int i = 0; i < n; i++) {
                        char line[128];
                        snprintf(line, sizeof(line), "%d|%s|%s|%s|%d\n",
                            buf[i].id, buf[i].username, buf[i].phone, buf[i].email, buf[i].role);
                        strncat(out, line, sizeof(out)-strlen(out)-1);
                    }
                    send_str(client_sockfd, CMD_USER_VIEW, out[0] ? out : "暂无用户");
                    break;
                }
                case CMD_USER_QUERY: {
                    int user_id;
                    memcpy(&user_id, packet.data, sizeof(int));
                    User u;
                    if (get_user_by_id(user_id, &u) == 0) {
                        char out[256];
                        snprintf(out, sizeof(out), "%d|%s|%s|%s|%d\n",
                            u.id, u.username, u.phone, u.email, u.role);
                        send_str(client_sockfd, CMD_USER_QUERY, out);
                    } else {
                        send_str(client_sockfd, CMD_USER_QUERY, "未找到该用户");
                    }
                    break;
                }

                // ======= 商品相关 =======
                case CMD_GOODS_ADD: {
                    Goods g;
                    memcpy(&g, packet.data, sizeof(Goods));
                    int new_id = add_goods(&g);
                    if (new_id > 0) send_str(client_sockfd, CMD_GOODS_ADD, "商品添加成功");
                    else send_str(client_sockfd, CMD_GOODS_ADD, "商品添加失败");
                    break;
                }
                case CMD_GOODS_DEL: {
                    int goods_id;
                    memcpy(&goods_id, packet.data, sizeof(int));
                    int ret = delete_goods(goods_id);
                    if (ret == 0) send_str(client_sockfd, CMD_GOODS_DEL, "商品删除成功");
                    else send_str(client_sockfd, CMD_GOODS_DEL, "商品删除失败");
                    break;
                }
                case CMD_GOODS_MODIFY: {
                    Goods g;
                    memcpy(&g, packet.data, sizeof(Goods));
                    int ret = update_goods(&g);
                    if (ret == 0) send_str(client_sockfd, CMD_GOODS_MODIFY, "商品修改成功");
                    else send_str(client_sockfd, CMD_GOODS_MODIFY, "商品修改失败");
                    break;
                }
                case CMD_GOODS_VIEW: {
                    Goods buf[1024];
                    int n = list_all_goods(buf, 1024);
                    char out[MAX_REPLY_BUF] = "";
                    for (int i = 0; i < n; i++) {
                        char line[256];
                        snprintf(line, sizeof(line), "%d|%s|%s|%.2f|%d\n",
                                 buf[i].id, buf[i].name, buf[i].category, buf[i].price, buf[i].stock);
                        strncat(out, line, sizeof(out)-strlen(out)-1);
                    }
                    send_str(client_sockfd, CMD_GOODS_VIEW, out[0] ? out : "暂无商品");
                    break;
                }
                case CMD_GOODS_QUERY: {
                    int goods_id;
                    memcpy(&goods_id, packet.data, sizeof(int));
                    Goods g;
                    if (get_goods_by_id(goods_id, &g) == 0) {
                        char out[256];
                        snprintf(out, sizeof(out), "%d|%s|%s|%.2f|%d\n",
                            g.id, g.name, g.category, g.price, g.stock);
                        send_str(client_sockfd, CMD_GOODS_QUERY, out);
                    } else {
                        send_str(client_sockfd, CMD_GOODS_QUERY, "未找到该商品");
                    }
                    break;
                }

                // ======= 订单相关 =======
                case CMD_ORDER_ADD: {
                    Order o;
                    memcpy(&o, packet.data, sizeof(Order));
                    int new_id = add_order(&o);
                    if (new_id > 0) send_str(client_sockfd, CMD_ORDER_ADD, "订单下单成功");
                    else send_str(client_sockfd, CMD_ORDER_ADD, "订单下单失败");
                    break;
                }
                case CMD_ORDER_DEL: {
                    int order_id;
                    memcpy(&order_id, packet.data, sizeof(int));
                    int ret = delete_order(order_id);
                    if (ret == 0) send_str(client_sockfd, CMD_ORDER_DEL, "订单删除成功");
                    else send_str(client_sockfd, CMD_ORDER_DEL, "订单删除失败");
                    break;
                }
                case CMD_ORDER_MODIFY: {
                    Order o;
                    memcpy(&o, packet.data, sizeof(Order));
                    int ret = update_order(&o);
                    if (ret == 0) send_str(client_sockfd, CMD_ORDER_MODIFY, "订单修改成功");
                    else send_str(client_sockfd, CMD_ORDER_MODIFY, "订单修改失败");
                    break;
                }
                case CMD_ORDER_VIEW: {
                    Order buf[1024];
                    int n = list_all_orders(buf, 1024);
                    char out[MAX_REPLY_BUF] = "";
                    for (int i = 0; i < n; i++) {
                        char line[256];
                        snprintf(line, sizeof(line), "%d|用户:%d|商品:%d|数量:%d|总价:%.2f|状态:%d|时间:%s\n",
                                 buf[i].id, buf[i].user_id, buf[i].goods_id, buf[i].quantity, buf[i].total_price, buf[i].status, buf[i].created_at);
                        strncat(out, line, sizeof(out)-strlen(out)-1);
                    }
                    send_str(client_sockfd, CMD_ORDER_VIEW, out[0] ? out : "暂无订单");
                    break;
                }
                case CMD_ORDER_QUERY: {
                    int order_id;
                    memcpy(&order_id, packet.data, sizeof(int));
                    Order o;
                    if (get_order_by_id(order_id, &o) == 0) {
                        char out[256];
                        snprintf(out, sizeof(out), "%d|用户:%d|商品:%d|数量:%d|总价:%.2f|状态:%d|时间:%s\n",
                                 o.id, o.user_id, o.goods_id, o.quantity, o.total_price, o.status, o.created_at);
                        send_str(client_sockfd, CMD_ORDER_QUERY, out);
                    } else {
                        send_str(client_sockfd, CMD_ORDER_QUERY, "未找到该订单");
                    }
                    break;
                }

                // ======= 购物车相关 =======
                case CMD_CART_ADD: {
                    CartItem c;
                    memcpy(&c, packet.data, sizeof(CartItem));
                    int new_id = add_cart(&c);
                    if (new_id > 0) send_str(client_sockfd, CMD_CART_ADD, "加入购物车成功");
                    else send_str(client_sockfd, CMD_CART_ADD, "加入购物车失败");
                    break;
                }
                case CMD_CART_DEL: {
                    int cart_id;
                    memcpy(&cart_id, packet.data, sizeof(int));
                    int ret = delete_cart(cart_id);
                    if (ret == 0) send_str(client_sockfd, CMD_CART_DEL, "删除购物车项成功");
                    else send_str(client_sockfd, CMD_CART_DEL, "删除购物车项失败");
                    break;
                }
                case CMD_CART_MODIFY: {
                    CartItem c;
                    memcpy(&c, packet.data, sizeof(CartItem));
                    int ret = update_cart(&c);
                    if (ret == 0) send_str(client_sockfd, CMD_CART_MODIFY, "修改购物车项成功");
                    else send_str(client_sockfd, CMD_CART_MODIFY, "修改购物车项失败");
                    break;
                }
                case CMD_CART_VIEW: {
                    int user_id;
                    memcpy(&user_id, packet.data, sizeof(int));
                    CartItem buf[1024];
                    int n = list_cart_by_user(user_id, buf, 1024);
                    char out[MAX_REPLY_BUF] = "";
                    for (int i = 0; i < n; i++) {
                        char line[128];
                        snprintf(line, sizeof(line), "购物车项%d|商品:%d|数量:%d\n", buf[i].id, buf[i].goods_id, buf[i].quantity);
                        strncat(out, line, sizeof(out)-strlen(out)-1);
                    }
                    send_str(client_sockfd, CMD_CART_VIEW, out[0] ? out : "购物车空");
                    break;
                }

                // ======= 收藏相关 =======
                case CMD_FAV_ADD: {
                    Favorite f;
                    memcpy(&f, packet.data, sizeof(Favorite));
                    int new_id = add_favorite(&f);
                    if (new_id > 0) send_str(client_sockfd, CMD_FAV_ADD, "收藏成功");
                    else send_str(client_sockfd, CMD_FAV_ADD, "收藏失败");
                    break;
                }
                case CMD_FAV_DEL: {
                    int fav_id;
                    memcpy(&fav_id, packet.data, sizeof(int));
                    int ret = delete_favorite(fav_id);
                    if (ret == 0) send_str(client_sockfd, CMD_FAV_DEL, "取消收藏成功");
                    else send_str(client_sockfd, CMD_FAV_DEL, "取消收藏失败");
                    break;
                }
                case CMD_FAV_VIEW: {
                    int user_id;
                    memcpy(&user_id, packet.data, sizeof(int));
                    Favorite buf[1024];
                    int n = list_favorites_by_user(user_id, buf, 1024);
                    char out[MAX_REPLY_BUF] = "";
                    for (int i = 0; i < n; i++) {
                        char line[128];
                        snprintf(line, sizeof(line), "收藏项%d|商品:%d\n", buf[i].id, buf[i].goods_id);
                        strncat(out, line, sizeof(out)-strlen(out)-1);
                    }
                    send_str(client_sockfd, CMD_FAV_VIEW, out[0] ? out : "暂无收藏");
                    break;
                }

                // ======= 店铺相关 =======
                case CMD_STORE_ADD: {
                    Store s;
                    memcpy(&s, packet.data, sizeof(Store));
                    int new_id = add_store(&s);
                    if (new_id > 0) send_str(client_sockfd, CMD_STORE_ADD, "店铺添加成功");
                    else send_str(client_sockfd, CMD_STORE_ADD, "店铺添加失败");
                    break;
                }
                case CMD_STORE_DEL: {
                    int store_id;
                    memcpy(&store_id, packet.data, sizeof(int));
                    int ret = delete_store(store_id);
                    if (ret == 0) send_str(client_sockfd, CMD_STORE_DEL, "店铺删除成功");
                    else send_str(client_sockfd, CMD_STORE_DEL, "店铺删除失败");
                    break;
                }
                case CMD_STORE_MODIFY: {
                    Store s;
                    memcpy(&s, packet.data, sizeof(Store));
                    int ret = update_store(&s);
                    if (ret == 0) send_str(client_sockfd, CMD_STORE_MODIFY, "店铺修改成功");
                    else send_str(client_sockfd, CMD_STORE_MODIFY, "店铺修改失败");
                    break;
                }
                case CMD_STORE_VIEW: {
                    Store buf[1024];
                    int n = list_all_stores(buf, 1024);
                    char out[MAX_REPLY_BUF] = "";
                    for (int i = 0; i < n; i++) {
                        char line[128];
                        snprintf(line, sizeof(line), "%d|%s|%s|%s|%d\n",
                                 buf[i].id, buf[i].name, buf[i].address, buf[i].phone, buf[i].owner_id);
                        strncat(out, line, sizeof(out)-strlen(out)-1);
                    }
                    send_str(client_sockfd, CMD_STORE_VIEW, out[0] ? out : "暂无店铺");
                    break;
                }
                case CMD_STORE_QUERY: {
                    int store_id;
                    memcpy(&store_id, packet.data, sizeof(int));
                    Store s;
                    if (get_store_by_id(store_id, &s) == 0) {
                        char out[256];
                        snprintf(out, sizeof(out), "%d|%s|%s|%s|%d\n",
                                 s.id, s.name, s.address, s.phone, s.owner_id);
                        send_str(client_sockfd, CMD_STORE_QUERY, out);
                    } else {
                        send_str(client_sockfd, CMD_STORE_QUERY, "未找到该店铺");
                    }
                    break;
                }

                // ======= 统计/历史扩展 =======
                case CMD_STAT_TOTAL: {
                    int user_id;
                    memcpy(&user_id, packet.data, sizeof(int));
                    double total = calc_user_total_spent(user_id);
                    char out[128];
                    snprintf(out, sizeof(out), "用户%d消费总额：%.2f元", user_id, total);
                    send_str(client_sockfd, CMD_STAT_TOTAL, out);
                    break;
                }
                case CMD_STAT_HISTORY: {
                    int user_id;
                    memcpy(&user_id, packet.data, sizeof(int));
                    Order buf[1024];
                    int n = get_user_order_history(user_id, buf, 1024);
                    char out[MAX_REPLY_BUF] = "";
                    for (int i = 0; i < n; i++) {
                        char line[256];
                        snprintf(line, sizeof(line), "订单%d|商品:%d|数量:%d|总价:%.2f|状态:%d|时间:%s\n",
                                 buf[i].id, buf[i].goods_id, buf[i].quantity, buf[i].total_price, buf[i].status, buf[i].created_at);
                        strncat(out, line, sizeof(out)-strlen(out)-1);
                    }
                    send_str(client_sockfd, CMD_STAT_HISTORY, out[0] ? out : "无消费历史");
                    break;
                }

                // ======= 默认 =======
                default:
                    send_str(client_sockfd, packet.cmd, "未知命令类型");
                    break;
            }
        } else if (recv_len == 0) {
            printf("连接断开，fd=%d，来自 %s:%d\n", client_sockfd, client_ip, client_port);
            break;
        } else {
            perror("recv failed");
            break;
        }
    }
done:
    close(client_sockfd);
    return NULL;
}