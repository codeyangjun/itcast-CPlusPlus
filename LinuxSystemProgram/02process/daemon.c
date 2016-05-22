#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


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
        exit(0);    
    }   
    else
    {
        pid = setsid();
        printf("pid=%d\n",pid);
        chdir("/");
        int i;
        for(i = 0; i < 3; i++)
            close(i);

        open("/dev/null",O_RDWR);
        dup(0);
        dup(0);
        while(1)
        {
        }
    }

    printf("after fork\n");
    return 0;
}

