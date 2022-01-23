#目前最通用，函数
CC:=gcc
SRCS:=$(wildcard *.c)   #拿到当前目录下所有的.c
TARGETS:=$(SRCS:%.c=%)
all:$(TARGETS)
%:%.c
	$(CC) -g $< -o $@
clean:
	rm -rf $(TARGETS)
