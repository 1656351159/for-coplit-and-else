#include "favorite_data.h"
#include <stdio.h>
#include <string.h>

#define FAVORITE_FILE "favorite.dat"

int add_favorite(const Favorite* f) {
    FILE* fp = fopen(FAVORITE_FILE, "ab+");
    if (!fp) return -1;
    fseek(fp, 0, SEEK_END);
    Favorite tmp = *f;
    long len = ftell(fp);
    tmp.id = (int)(len / sizeof(Favorite)) + 1;
    int ret = fwrite(&tmp, sizeof(Favorite), 1, fp);
    fclose(fp);
    return ret == 1 ? tmp.id : -1;
}

int delete_favorite(int fav_id) {
    FILE* fp = fopen(FAVORITE_FILE, "rb");
    FILE* fp2 = fopen("favorite.tmp", "wb");
    if (!fp || !fp2) { if(fp) fclose(fp); if(fp2) fclose(fp2); return -1; }
    Favorite f;
    int found = 0;
    while (fread(&f, sizeof(Favorite), 1, fp)) {
        if (f.id != fav_id) fwrite(&f, sizeof(Favorite), 1, fp2);
        else found = 1;
    }
    fclose(fp); fclose(fp2);
    remove(FAVORITE_FILE); rename("favorite.tmp", FAVORITE_FILE);
    return found ? 0 : -1;
}

int list_favorites_by_user(int user_id, Favorite* buf, int max_count) {
    FILE* fp = fopen(FAVORITE_FILE, "rb");
    if (!fp) return 0;
    int cnt = 0;
    Favorite f;
    while (cnt < max_count && fread(&f, sizeof(Favorite), 1, fp)) {
        if (f.user_id == user_id) buf[cnt++] = f;
    }
    fclose(fp);
    return cnt;
}