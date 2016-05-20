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
    int fd;
    fd = open("test.txt",O_WRONLY);
    if(fd == -1)
    {
        perror("open error");
        exit(0);
    }

    pid = fork();
    if(pid < 0)
    {
        printf("error\n");
        return 0;
    }

    if(pid >0)
    {
        printf("this is parent process, pid = %d\n", getpid());
        write(fd,"parent",6);
        close(fd);
    }
    else
    {
        write(fd,"child",5);
       /* close(fd);*/
        printf("this is child procee, pid = %d\n", getpid());
    }

    printf("after fork\n");
    sleep(10);
    return 0;
}

