#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    printf("hello\n");

    pid_t pid;
    printf("before fork,pid=%d\n", getpid());
    pid = fork();

    if(pid < 0)
    {
        printf("error\n");
        return 0;
    }

    if(pid >0)
    {
        printf("this is parent process, pid = %d\n", getpid());
    }
    else
    {
        printf("this is child procee, pid = %d\n", getpid());
    }

    printf("after fork\n");
    return 0;
}

