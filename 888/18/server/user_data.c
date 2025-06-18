#include "user_data.h"
#include <stdio.h>
#include <string.h>

#define USER_FILE "user.dat"

int add_user(const User* u) {
    FILE* fp = fopen(USER_FILE, "ab+");
    if (!fp) return -1;
    fseek(fp, 0, SEEK_END);
    User tmp = *u;
    long len = ftell(fp);
    tmp.id = (int)(len / sizeof(User)) + 1;
    int ret = fwrite(&tmp, sizeof(User), 1, fp);
    fclose(fp);
    return ret == 1 ? tmp.id : -1;
}

int delete_user(int user_id) {
    FILE* fp = fopen(USER_FILE, "rb");
    FILE* fp2 = fopen("user.tmp", "wb");
    if (!fp || !fp2) { if(fp) fclose(fp); if(fp2) fclose(fp2); return -1; }
    User u;
    int found = 0;
    while (fread(&u, sizeof(User), 1, fp)) {
        if (u.id != user_id) fwrite(&u, sizeof(User), 1, fp2);
        else found = 1;
    }
    fclose(fp); fclose(fp2);
    remove(USER_FILE); rename("user.tmp", USER_FILE);
    return found ? 0 : -1;
}

int update_user(const User* u) {
    FILE* fp = fopen(USER_FILE, "rb+");
    if (!fp) return -1;
    User tmp;
    int found = 0;
    while (fread(&tmp, sizeof(User), 1, fp)) {
        if (tmp.id == u->id) {
            fseek(fp, -sizeof(User), SEEK_CUR);
            fwrite(u, sizeof(User), 1, fp);
            found = 1;
            break;
        }
    }
    fclose(fp);
    return found ? 0 : -1;
}

int get_user_by_id(int user_id, User* out) {
    FILE* fp = fopen(USER_FILE, "rb");
    if (!fp) return -1;
    User u;
    int found = 0;
    while (fread(&u, sizeof(User), 1, fp)) {
        if (u.id == user_id) {
            if (out) *out = u;
            found = 1;
            break;
        }
    }
    fclose(fp);
    return found ? 0 : -1;
}

int get_user_by_name(const char* username, User* out) {
    FILE* fp = fopen(USER_FILE, "rb");
    if (!fp) return -1;
    User u;
    int found = 0;
    while (fread(&u, sizeof(User), 1, fp)) {
        if (strcmp(u.username, username) == 0) {
            if (out) *out = u;
            found = 1;
            break;
        }
    }
    fclose(fp);
    return found ? 0 : -1;
}

int list_all_users(User* buf, int max_count) {
    FILE* fp = fopen(USER_FILE, "rb");
    if (!fp) return 0;
    int cnt = 0;
    while (cnt < max_count && fread(&buf[cnt], sizeof(User), 1, fp)) cnt++;
    fclose(fp);
    return cnt;
}