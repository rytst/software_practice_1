###
###  Makefile: calc プログラムの Makefile
###

# コンパイル フラグ
#CC = cc 
CC = gcc 
CFLAGS = -g -Wall -pedantic

OBJS = main.o lexer.o parse.o
TARGET = calc

# 依存関係とターゲットの構築ルール
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LIBRARIES)

$(OBJS): calc.h 

clean: 
	rm -f $(OBJS) *~ $(TARGET) 
