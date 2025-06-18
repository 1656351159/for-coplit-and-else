
# 客户端 Makefile

CC = gcc
CFLAGS = -Wall -g -I../common

SRCS = client.c client_util.c
OBJS = $(SRCS:.c=.o)

TARGET = client

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean