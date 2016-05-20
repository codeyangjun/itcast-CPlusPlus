#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main()
{

printf("I am hello:%d\n", getpid());
    return 0;
}

