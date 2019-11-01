CC=g++
CFLAGS=-Wall -Werror
all: ans
ans: main.o getTileNonetool.o
	$(CC) $(CFLAGS) $? -o $@
.cpp.o:
	$(CC) $(CFLAGS) -c $*.cpp