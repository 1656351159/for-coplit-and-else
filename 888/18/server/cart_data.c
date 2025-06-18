#include "cart_data.h"
#include <stdio.h>
#include <string.h>

#define CART_FILE "cart.dat"

int add_cart(const CartItem* c) {
    FILE* fp = fopen(CART_FILE, "ab+");
    if (!fp) return -1;
    fseek(fp, 0, SEEK_END);
    CartItem tmp = *c;
    long len = ftell(fp);
    tmp.id = (int)(len / sizeof(CartItem)) + 1;
    int ret = fwrite(&tmp, sizeof(CartItem), 1, fp);
    fclose(fp);
    return ret == 1 ? tmp.id : -1;
}

int delete_cart(int cart_id) {
    FILE* fp = fopen(CART_FILE, "rb");
    FILE* fp2 = fopen("cart.tmp", "wb");
    if (!fp || !fp2) { if(fp) fclose(fp); if(fp2) fclose(fp2); return -1; }
    CartItem c;
    int found = 0;
    while (fread(&c, sizeof(CartItem), 1, fp)) {
        if (c.id != cart_id) fwrite(&c, sizeof(CartItem), 1, fp2);
        else found = 1;
    }
    fclose(fp); fclose(fp2);
    remove(CART_FILE); rename("cart.tmp", CART_FILE);
    return found ? 0 : -1;
}

int update_cart(const CartItem* c) {
    FILE* fp = fopen(CART_FILE, "rb+");
    if (!fp) return -1;
    CartItem tmp;
    int found = 0;
    while (fread(&tmp, sizeof(CartItem), 1, fp)) {
        if (tmp.id == c->id) {
            fseek(fp, -sizeof(CartItem), SEEK_CUR);
            fwrite(c, sizeof(CartItem), 1, fp);
            found = 1;
            break;
        }
    }
    fclose(fp);
    return found ? 0 : -1;
}

int get_cart_by_id(int cart_id, CartItem* out) {
    FILE* fp = fopen(CART_FILE, "rb");
    if (!fp) return -1;
    CartItem c;
    int found = 0;
    while (fread(&c, sizeof(CartItem), 1, fp)) {
        if (c.id == cart_id) {
            if (out) *out = c;
            found = 1;
            break;
        }
    }
    fclose(fp);
    return found ? 0 : -1;
}

int list_cart_by_user(int user_id, CartItem* buf, int max_count) {
    FILE* fp = fopen(CART_FILE, "rb");
    if (!fp) return 0;
    int cnt = 0;
    CartItem c;
    while (cnt < max_count && fread(&c, sizeof(CartItem), 1, fp)) {
        if (c.user_id == user_id) buf[cnt++] = c;
    }
    fclose(fp);
    return cnt;
}