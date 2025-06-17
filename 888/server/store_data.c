#include "store_data.h"
#include <stdio.h>
#include <string.h>

#define STORE_FILE "store.dat"

int add_store(const Store* s) {
    FILE* fp = fopen(STORE_FILE, "ab+");
    if (!fp) return -1;
    fseek(fp, 0, SEEK_END);
    Store tmp = *s;
    long len = ftell(fp);
    tmp.id = (int)(len / sizeof(Store)) + 1;
    int ret = fwrite(&tmp, sizeof(Store), 1, fp);
    fclose(fp);
    return ret == 1 ? tmp.id : -1;
}

int delete_store(int store_id) {
    FILE* fp = fopen(STORE_FILE, "rb");
    FILE* fp2 = fopen("store.tmp", "wb");
    if (!fp || !fp2) { if(fp) fclose(fp); if(fp2) fclose(fp2); return -1; }
    Store s;
    int found = 0;
    while (fread(&s, sizeof(Store), 1, fp)) {
        if (s.id != store_id) fwrite(&s, sizeof(Store), 1, fp2);
        else found = 1;
    }
    fclose(fp); fclose(fp2);
    remove(STORE_FILE); rename("store.tmp", STORE_FILE);
    return found ? 0 : -1;
}

int update_store(const Store* s) {
    FILE* fp = fopen(STORE_FILE, "rb+");
    if (!fp) return -1;
    Store tmp;
    int found = 0;
    while (fread(&tmp, sizeof(Store), 1, fp)) {
        if (tmp.id == s->id) {
            fseek(fp, -sizeof(Store), SEEK_CUR);
            fwrite(s, sizeof(Store), 1, fp);
            found = 1;
            break;
        }
    }
    fclose(fp);
    return found ? 0 : -1;
}

int get_store_by_id(int store_id, Store* out) {
    FILE* fp = fopen(STORE_FILE, "rb");
    if (!fp) return -1;
    Store s;
    int found = 0;
    while (fread(&s, sizeof(Store), 1, fp)) {
        if (s.id == store_id) {
            if (out) *out = s;
            found = 1;
            break;
        }
    }
    fclose(fp);
    return found ? 0 : -1;
}

int list_all_stores(Store* buf, int max_count) {
    FILE* fp = fopen(STORE_FILE, "rb");
    if (!fp) return 0;
    int cnt = 0;
    while (cnt < max_count && fread(&buf[cnt], sizeof(Store), 1, fp)) cnt++;
    fclose(fp);
    return cnt;
}