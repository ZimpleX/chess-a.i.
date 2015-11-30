CC=g++
CFLAGS=

.PHONY : all
all : main.o

main.o : algo.h rule.h util.h test_board.h main.cpp util.o rule.o algo.o
	$(CC) $(CFLAGS) util.o rule.o algo.o main.cpp -o main.o

util.o : util.h util.cpp rule.h
	$(CC) $(CFLAGS) -c util.cpp

rule.o : rule.cpp rule.h util.h
	$(CC) $(CFLAGS) -c rule.cpp

algo.o : algo.cpp algo.h util.h rule.h
	$(CC) $(CFLAGS) -c algo.cpp
