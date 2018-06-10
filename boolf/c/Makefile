SRC=main.c boolf/boolf.c stack/stack.c buffer/buffer.c
CFLAGS=-Wall -O3 -Wpedantic -Werror -std=c11 -I./stack -I./boolf -I./buffer
OFNAME=main

all:
	@gcc ${CFLAGS} ${SRC} -o ${OFNAME}

clean:
	@rm -f ${OFNAME} *.o
