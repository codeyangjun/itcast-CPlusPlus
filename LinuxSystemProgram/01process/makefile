

.PHONY:clean all
CC=gcc
CFLAGS=-Wall -g
BIN=fork_01 mulfork orphanThread forkFile
all:${BIN}

%.o:%.c
	${CC} ${CFLAGS} -c $< -o $@

clean:
	rm -f *.o ${BIN}
