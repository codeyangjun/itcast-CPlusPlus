#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>


int main()
{
    pid_t pid;
    pid = fork();

    if(pid < 0)
    {
        printf("error\n");
        return 0;
    }

    if(pid >0)
    {
       // exit(0); 
       sleep(1);
    }

    if(pid == 0) 
    {
        //sleep(3);
        printf("this is child proces:\n");
        exit(100);
        //abort();
    }

    int ret;
    printf("after fork,this is parent process\n");
    int status;
    //ret = wait(&status);
    ret = waitpid(pid, &status, 0);
    printf("ret = %d pid = %d\n", ret, pid);
    if( WIFEXITED(status))
    {
        printf("进程正常退出：%d\n", WEXITSTATUS(status) );
    }
    else if(WIFSIGNALED(status))
    {
        printf("进程非正常退出:%d\n", WTERMSIG(status) );
    }
    else if(WIFSTOPPED(status))
    {
        printf("进程停止:%d\n", WSTOPSIG(status));
    }
    else
    {
        printf("其它方式退出\n");
    }

    return 0;
}

