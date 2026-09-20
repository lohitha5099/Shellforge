CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude

TARGET = shellforge

SRCS = src/main.c src/lexer.c src/token.c src/parser.c src/expand.c src/builtin.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

src/main.o: src/main.c include/lexer.h include/token.h include/parser.h include/expand.h include/builtin.h
	$(CC) $(CFLAGS) -c src/main.c -o src/main.o

src/lexer.o: src/lexer.c include/lexer.h include/token.h
	$(CC) $(CFLAGS) -c src/lexer.c -o src/lexer.o

src/token.o: src/token.c include/token.h
	$(CC) $(CFLAGS) -c src/token.c -o src/token.o

src/parser.o: src/parser.c include/parser.h include/token.h
	$(CC) $(CFLAGS) -c src/parser.c -o src/parser.o

src/expand.o: src/expand.c include/expand.h include/parser.h
	$(CC) $(CFLAGS) -c src/expand.c -o src/expand.o

src/builtin.o: src/builtin.c include/builtin.h
	$(CC) $(CFLAGS) -c src/builtin.c -o src/builtin.o

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
