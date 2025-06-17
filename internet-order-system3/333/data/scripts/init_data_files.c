// 用于初始化数据目录下的数据文件（goods.dat、users.dat、orders.dat）
// 运行一次即可，确保 data/ 目录存在

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 空结构体定义，实际内容以项目 types.h/protocol.h 为准
typedef struct {
    int dummy; // 占位
} Goods, User, Order;

int main() {
    FILE* fp;

    // 初始化 goods.dat
    fp = fopen("data/goods.dat", "wb");
    if (!fp) { perror("goods.dat"); exit(1); }
    // 可以写入一个空文件，或者写入初始数据
    fclose(fp);

    // 初始化 users.dat
    fp = fopen("data/users.dat", "wb");
    if (!fp) { perror("users.dat"); exit(1); }
    fclose(fp);

    // 初始化 orders.dat
    fp = fopen("data/orders.dat", "wb");
    if (!fp) { perror("orders.dat"); exit(1); }
    fclose(fp);

    printf("数据文件已初始化（data/goods.dat, data/users.dat, data/orders.dat）\n");
    return 0;
}