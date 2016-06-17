.PHONY:clean all
CC=gcc
CFLAGS=-Wall -g
BIN=exam1 exam2 exam3
all:${BIN}

%.o:%.c
	${CC} ${CFLAGS} -c $< -o $@

clean:
	rm -f *.o ${BIN}
