SRC=main.c
CFLAGS=-Wall -g -Wpedantic -Werror -std=c11
OFNAME=main

all:
	@gcc ${CFLAGS} ${SRC} -o ${OFNAME} -lm

clean:
	@rm -f ${OFNAME} *.o
