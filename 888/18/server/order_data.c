#include "order_data.h"
#include <stdio.h>
#include <string.h>

#define ORDER_FILE "order.dat"

int add_order(const Order* o) {
    FILE* fp = fopen(ORDER_FILE, "ab+");
    if (!fp) return -1;
    fseek(fp, 0, SEEK_END);
    Order tmp = *o;
    long len = ftell(fp);
    tmp.id = (int)(len / sizeof(Order)) + 1;
    int ret = fwrite(&tmp, sizeof(Order), 1, fp);
    fclose(fp);
    return ret == 1 ? tmp.id : -1;
}

int delete_order(int order_id) {
    FILE* fp = fopen(ORDER_FILE, "rb");
    FILE* fp2 = fopen("order.tmp", "wb");
    if (!fp || !fp2) { if(fp) fclose(fp); if(fp2) fclose(fp2); return -1; }
    Order o;
    int found = 0;
    while (fread(&o, sizeof(Order), 1, fp)) {
        if (o.id != order_id) fwrite(&o, sizeof(Order), 1, fp2);
        else found = 1;
    }
    fclose(fp); fclose(fp2);
    remove(ORDER_FILE); rename("order.tmp", ORDER_FILE);
    return found ? 0 : -1;
}

int update_order(const Order* o) {
    FILE* fp = fopen(ORDER_FILE, "rb+");
    if (!fp) return -1;
    Order tmp;
    int found = 0;
    while (fread(&tmp, sizeof(Order), 1, fp)) {
        if (tmp.id == o->id) {
            fseek(fp, -sizeof(Order), SEEK_CUR);
            fwrite(o, sizeof(Order), 1, fp);
            found = 1;
            break;
        }
    }
    fclose(fp);
    return found ? 0 : -1;
}

int get_order_by_id(int order_id, Order* out) {
    FILE* fp = fopen(ORDER_FILE, "rb");
    if (!fp) return -1;
    Order o;
    int found = 0;
    while (fread(&o, sizeof(Order), 1, fp)) {
        if (o.id == order_id) {
            if (out) *out = o;
            found = 1;
            break;
        }
    }
    fclose(fp);
    return found ? 0 : -1;
}

int list_all_orders(Order* buf, int max_count) {
    FILE* fp = fopen(ORDER_FILE, "rb");
    if (!fp) return 0;
    int cnt = 0;
    while (cnt < max_count && fread(&buf[cnt], sizeof(Order), 1, fp)) cnt++;
    fclose(fp);
    return cnt;
}