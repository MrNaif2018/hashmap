# build files main.c hashmap.c mylib.c

# Variables
CC=gcc
GXX=g++
CFLAGS=-Wall -Werror -Wextra -pedantic -O2
VALGRIND=valgrind --leak-check=full --show-leak-kinds=all

# Phony targets
.PHONY: all clean debug memcheck

# Default target
all: main groupstats benchmark

# Build main executable
main: main.o hashmap.o mylib.o
	$(CC) $(CFLAGS) -o main $^

groupstats: groupstats.cpp
	$(GXX) $(CFLAGS) -o groupstats groupstats.cpp

benchmark: hashmap.cpp myhashmap.c mylib.o hashmap.o
	$(GXX) $(CFLAGS) -o hashmap hashmap.cpp
	$(CC) $(CFLAGS) -o myhashmap myhashmap.c mylib.o hashmap.o

runbench:
	./hashmap
	./myhashmap

# Build main executable with debug symbols
debug: main.o hashmap.o mylib.o
	$(CC) $(CFLAGS) -o main $^ -g

# Build main.o
main.o: main.c hashmap.h mylib.h
	$(CC) $(CFLAGS) -c main.c

# Build hashmap.o
hashmap.o: hashmap.c hashmap.h
	$(CC) $(CFLAGS) -c hashmap.c

# Build mylib.o
mylib.o: mylib.c mylib.h
	$(CC) $(CFLAGS) -c mylib.c

# Clean all build files
clean:
	rm -f main groupstats hashmap myhashmap *.o