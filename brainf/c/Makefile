SRC=main.c brainf/brainf.c stack/stack.c buffer/buffer.c
CFLAGS=-Wall -O3 -Wpedantic -Werror -std=c11 -I./stack -I./brainf -I./buffer
OFNAME=main

all:
	@gcc ${CFLAGS} ${SRC} -o ${OFNAME}

clean:
	@rm -f ${OFNAME} *.o
