# ========== 通用 Makefile，适用于 server 目录 ==========
# 支持 pthread，防止多重定义，结构清晰

# 编译器与参数
CC = gcc
CFLAGS = -Wall -g -I../common

# 源文件列表（根据你的服务器目录实际文件列出）
SRCS = main.c handler.c net_util.c user.c user_data.c goods_data.c order_data.c store_data.c favorite_data.c cart_data.c stat_data.c

# 如果你确定 data_store.c 没有和其他 .c 文件重复的实现，可以加上
# 否则，建议先注释掉 data_store.c 以避免多重定义
# SRCS += data_store.c

OBJS = $(SRCS:.c=.o)

TARGET = server

# 默认目标
all: $(TARGET)

# 链接时加 -lpthread 以支持多线程
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -lpthread

# 生成 .o 文件
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 清理目标
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean