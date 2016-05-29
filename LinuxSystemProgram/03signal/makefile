
.PHONY:clean all
CC=gcc
CFLAGS=-Wall -g
BIN=signal1 signal2 signal3 alarm sigstatus sigaction sigqueue
all:${BIN}

%.o:%.c
	${CC} ${CFLAGS} -c $< -o $@

clean:
	rm -f *.o ${BIN}
