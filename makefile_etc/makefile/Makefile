#目前最通用，函数
out:=main
SRCS:=$(wildcard *.c)   #拿到当前目录下所有的.c
OBJS:=$(patsubst %.c,%.o,$(SRCS)) #替换SRCS的.c为.o
#预定义变量 CC CFLAGS
CC:=gcc
CFLAGS:=-g
#[目标文件]:[依赖文件]
$(out):$(OBJS)
	gcc $^ -o $@
#自动找main.o, func.o对应的c文件 cc-->gcc (line 4)
#.PHONY:clean  // 伪目标
rebuild:clean $(out) #先clean在main
clean:
	rm -rf $(OBJS) $(out)
