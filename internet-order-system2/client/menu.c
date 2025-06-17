#include "menu.h"
#include <stdio.h>

//三种身份 master shoper customer
//master 管理员 shoper 商家 customer 顾客

//1. 管理员主菜单
void show_master_menu() {
    printf("【管理员菜单】\n");
    printf("1. 首页\n");
    printf("2. 商品管理\n");
    printf("3. 店铺管理\n");
    printf("4. 店铺收藏\n");
    printf("5. 用户管理\n");
    printf("6. 订单管理\n");
    printf("7. 个人中心\n");
    printf("0. 退出系统\n");
}
// 商品管理菜单
void show_master_goods_menu() {
    printf("【商品管理】\n");
    printf("1. 添加商品\n");
    printf("2. 删除商品\n");
    printf("3. 修改商品\n");
    printf("4. 查看商品\n");
    printf("0. 返回上一级\n");
}
// 店铺管理菜单
void show_master_store_menu() {
    printf("【店铺管理】\n");
    printf("1. 添加店铺\n");
    printf("2. 删除店铺\n");
    printf("3. 修改店铺\n");
    printf("4. 查看店铺\n");
    printf("0. 返回上一级\n");
}
// 店铺收藏管理
void show_master_store_collect_menu() {
    printf("【店铺收藏】\n");
    printf("1. 查看收藏的店铺\n");
    printf("2. 删除收藏的店铺\n");
    printf("0. 返回上一级\n");
}
// 用户管理
void show_master_user_menu() {
    printf("【用户管理】\n");
    printf("1. 查看用户\n");
    printf("2. 删除用户\n");
    printf("3. 修改用户信息\n");
    printf("0. 返回上一级\n");
}
// 订单管理
void show_master_order_menu() {
    printf("【订单管理】\n");
    printf("1. 查看订单\n");
    printf("2. 删除订单\n");
    printf("3. 修改订单状态\n");
    printf("0. 返回上一级\n");
}
// 个人中心
void show_master_profile_menu() {
    printf("【个人中心】\n");
    printf("1. 修改个人信息\n");
    printf("2. 查看个人信息\n");
    printf("3. 修改密码\n");
    printf("4. 填写/修改地址\n");
    printf("5. 查看评论\n");
    printf("0. 返回上一级\n");
}
// 首页
void show_home_menu() {
    printf("【首页】\n");
    printf("1. 商品分类\n");
    printf("2. 商品收藏\n");
    printf("3. 搜索商品\n");
    printf("0. 返回上一级\n");
}
void show_category_menu() {
    printf("【商品分类】\n");
    printf("1. 中餐\n");
    printf("2. 西餐\n");
    printf("3. 快餐\n");
    printf("4. 饮品\n");
    printf("5. 拼好饭\n");
    printf("0. 返回上一级\n");
}
void show_chinese_food() {
    printf("【中餐商品】\n");
    printf("1. 麻婆豆腐\n");
    printf("2. 红烧肉\n");
    printf("0. 返回上一级\n");
}
void show_western_food() {
    printf("【西餐商品】\n");
    printf("1. 牛排\n");
    printf("0. 返回上一级\n");
}
void show_fast_food() {
    printf("【快餐商品】\n");
    printf("1. 汉堡\n");
    printf("2. 薯条\n");
    printf("0. 返回上一级\n");
}
void show_drinks() {
    printf("【饮品商品】\n");
    printf("1. 可乐\n");
    printf("2. 奶茶\n");
    printf("0. 返回上一级\n");
}
void show_rice_bowl() {
    printf("【拼好饭商品】\n");
    printf("1. 3.9的茶百道\n");
    printf("2. 3.9的华莱士\n");
    printf("0. 返回上一级\n");
}
void show_favorite_items() {
    printf("【商品收藏列表】\n");
}
void show_delete_favorite() {
    printf("【删除商品收藏】\n");
}
void show_search_menu() {
    printf("【搜索商品】\n");
    printf("1. 选择搜索\n");
    printf("2. 搜索历史\n");
    printf("0. 返回上一级\n");
}
void show_search_items() {
    printf("请输入你想搜索的商品：\n");
}
void show_search_history() {
    printf("【搜索历史列表】\n");
}

// 商家菜单
void show_shoper_menu() {
    printf("【商家菜单】\n");
    printf("1. 首页\n");
    printf("2. 商品管理\n");
    printf("3. 订单管理\n");
    printf("4. 个人中心\n");
    printf("0. 退出系统\n");
}
void show_shoper_goods_menu() {
    printf("【商品管理】\n");
    printf("1. 添加商品\n");
    printf("2. 删除商品\n");
    printf("3. 修改商品\n");
    printf("4. 查看商品\n");
    printf("0. 返回上一级\n");
}
void show_shoper_order_menu() {
    printf("【订单管理】\n");
    printf("1. 查看订单\n");
    printf("2. 修改订单状态\n");
    printf("0. 返回上一级\n");
}
void show_shoper_profile_menu() {
    printf("【个人中心】\n");
    printf("1. 修改个人信息\n");
    printf("2. 查看个人信息\n");
    printf("3. 修改密码\n");
    printf("4. 填写/修改地址\n");
    printf("5. 查看评论\n");
    printf("0. 返回上一级\n");
}

// 顾客菜单
void show_customer_menu() {
    printf("【顾客菜单】\n");
    printf("1. 首页\n");
    printf("2. 商品收藏\n");
    printf("3. 购物车\n");
    printf("4. 订单管理\n");
    printf("5. 个人中心\n");
    printf("0. 退出系统\n");
}
void show_customer_favorite_menu() {
    printf("【商品收藏】\n");
    printf("1. 查看收藏的商品\n");
    printf("2. 删除收藏的商品\n");
    printf("0. 返回上一级\n");
}
void show_customer_cart_menu() {
    printf("【购物车】\n");
    printf("1. 查看购物车\n");
    printf("2. 删除购物车商品\n");
    printf("3. 修改购物车商品数量\n");
    printf("0. 返回上一级\n");
}
void show_customer_order_menu() {
    printf("【订单管理】\n");
    printf("1. 查看订单\n");
    printf("2. 删除订单\n");
    printf("3. 修改订单状态\n");
    printf("0. 返回上一级\n");
}
void show_customer_profile_menu() {
    printf("【个人中心】\n");
    printf("1. 修改个人信息\n");
    printf("2. 查看个人信息\n");
    printf("3. 修改密码\n");
    printf("4. 填写/修改地址\n");
    printf("5. 查看评论\n");
    printf("0. 返回上一级\n");
}