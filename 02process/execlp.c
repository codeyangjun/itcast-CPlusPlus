#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main()
{
   // char* const argv = {"ls","-lt"};
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
    }
    else
    {
        printf("this is child procee, pid = %d\n", getpid());
       //     execve("./hello",NULL,NULL);
        //execve("/bin/ls", "ls -a", NULL);
        //int execle(const char *path, const char *arg,
                                 // ..., char * const envp[]);
        execlp("ls","ls","-lt",NULL);
        exit(0);
    }

    printf("after fork\n");
    return 0;
}

