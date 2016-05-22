

.PHONY:clean all
CC=gcc
CFLAGS=-Wall -g
BIN=vfork hello execlp daemon
all:${BIN}

%.o:%.c
	${CC} ${CFLAGS} -c $< -o $@

clean:
	rm -f *.o ${BIN}
