
.PHONY:clean all
CC=gcc
CFLAGS=-Wall -g -std=c99
BIN=server client server-1 client-1 server-2 client-2
all:${BIN}

%.o:%.c
	${CC} ${CFLAGS} -c $< -o $@

clean:
	rm -f *.o ${BIN}
