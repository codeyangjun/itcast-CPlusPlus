#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>


int main()
{

    pid_t pid;
    signal(SIGCHLD,SIG_IGN);
    pid = fork();
    if(pid < 0)
    {
        printf("error\n");
        return 0;
    }
    if(pid >0)
    {
        printf("this is parent process, pid = %d\n", getpid());
        sleep(100);
    }
    else
    {
        printf("this is child procee, pid = %d\n", getpid());
    }

    printf("after fork\n");
    return 0;
}

