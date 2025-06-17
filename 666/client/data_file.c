#include "data_file.h"
#include <stdio.h>
#include <string.h>

// 用户
int read_users(User *arr, int max_count) {
    FILE *fp = fopen("user.dat", "r");
    if (!fp) return 0;
    int count = 0;
    char line[256];
    while (fgets(line, sizeof(line), fp) && count < max_count) {
        if (sscanf(line, "%d|%31[^|]|%31[^|]|%63[^\n]", 
            &arr[count].id, arr[count].username, arr[count].phone, arr[count].email) == 4) {
            count++;
        }
    }
    fclose(fp);
    return count;
}
int write_users(const User *arr, int count) {
    FILE *fp = fopen("user.dat", "w");
    if (!fp) return 0;
    for (int i = 0; i < count; i++) {
        fprintf(fp, "%d|%s|%s|%s\n", arr[i].id, arr[i].username, arr[i].phone, arr[i].email);
    }
    fclose(fp);
    return 1;
}

// 商品
int read_goods(Goods *arr, int max_count) {
    FILE *fp = fopen("goods.dat", "r");
    if (!fp) return 0;
    int count = 0;
    char line[256];
    while (fgets(line, sizeof(line), fp) && count < max_count) {
        if (sscanf(line, "%d|%63[^|]|%d|%f|%d",
            &arr[count].id, arr[count].name, &arr[count].category, &arr[count].price, &arr[count].stock) == 5) {
            count++;
        }
    }
    fclose(fp);
    return count;
}
int write_goods(const Goods *arr, int count) {
    FILE *fp = fopen("goods.dat", "w");
    if (!fp) return 0;
    for (int i = 0; i < count; i++) {
        fprintf(fp, "%d|%s|%d|%.2f|%d\n", arr[i].id, arr[i].name, arr[i].category, arr[i].price, arr[i].stock);
    }
    fclose(fp);
    return 1;
}

// 订单
int read_orders(Order *arr, int max_count) {
    FILE *fp = fopen("order.dat", "r");
    if (!fp) return 0;
    int count = 0;
    char line[256];
    while (fgets(line, sizeof(line), fp) && count < max_count) {
        if (sscanf(line, "%d|%d|%d|%d|%31[^\n]",
            &arr[count].order_id, &arr[count].user_id, &arr[count].goods_id, &arr[count].quantity, arr[count].status) == 5) {
            count++;
        }
    }
    fclose(fp);
    return count;
}
int write_orders(const Order *arr, int count) {
    FILE *fp = fopen("order.dat", "w");
    if (!fp) return 0;
    for (int i = 0; i < count; i++) {
        fprintf(fp, "%d|%d|%d|%d|%s\n", arr[i].order_id, arr[i].user_id, arr[i].goods_id, arr[i].quantity, arr[i].status);
    }
    fclose(fp);
    return 1;
}

// 收藏
int read_favorites(Favorite *arr, int max_count) {
    FILE *fp = fopen("favorite.dat", "r");
    if (!fp) return 0;
    int count = 0;
    char line[128];
    while (fgets(line, sizeof(line), fp) && count < max_count) {
        if (sscanf(line, "%d|%d", &arr[count].user_id, &arr[count].goods_id) == 2) {
            count++;
        }
    }
    fclose(fp);
    return count;
}
int write_favorites(const Favorite *arr, int count) {
    FILE *fp = fopen("favorite.dat", "w");
    if (!fp) return 0;
    for (int i = 0; i < count; i++) {
        fprintf(fp, "%d|%d\n", arr[i].user_id, arr[i].goods_id);
    }
    fclose(fp);
    return 1;
}

// 购物车
int read_cart(Cart *arr, int max_count) {
    FILE *fp = fopen("cart.dat", "r");
    if (!fp) return 0;
    int count = 0;
    char line[128];
    while (fgets(line, sizeof(line), fp) && count < max_count) {
        if (sscanf(line, "%d|%d|%d", &arr[count].user_id, &arr[count].goods_id, &arr[count].quantity) == 3) {
            count++;
        }
    }
    fclose(fp);
    return count;
}
int write_cart(const Cart *arr, int count) {
    FILE *fp = fopen("cart.dat", "w");
    if (!fp) return 0;
    for (int i = 0; i < count; i++) {
        fprintf(fp, "%d|%d|%d\n", arr[i].user_id, arr[i].goods_id, arr[i].quantity);
    }
    fclose(fp);
    return 1;
}

// 店铺
int read_stores(Store *arr, int max_count) {
    FILE *fp = fopen("store.dat", "r");
    if (!fp) return 0;
    int count = 0;
    char line[256];
    while (fgets(line, sizeof(line), fp) && count < max_count) {
        if (sscanf(line, "%d|%63[^|]|%127[^\n]", &arr[count].store_id, arr[count].name, arr[count].address) == 3) {
            count++;
        }
    }
    fclose(fp);
    return count;
}
int write_stores(const Store *arr, int count) {
    FILE *fp = fopen("store.dat", "w");
    if (!fp) return 0;
    for (int i = 0; i < count; i++) {
        fprintf(fp, "%d|%s|%s\n", arr[i].store_id, arr[i].name, arr[i].address);
    }
    fclose(fp);
    return 1;
}