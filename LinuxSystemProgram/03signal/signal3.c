#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
void handler(int num)

{
    if(num == SIGINT)
    {
        printf("recv signal SIGINT\n");
    }
    else if(num == SIGUSR1)
    {
        printf("recv signal SIGUSR1\n");
    }
    else
    {
        printf("recv num:%d\n", num);
    }
}

//信号异步通知事件
//如果我写了信号中断处理函数handler，当信号发生时，会自动调用我写的函数
int main()
{
     printf("main...begin\n");
     pid_t pid;

     if(signal(SIGINT, handler) == SIG_ERR)
     {
        printf("func signal err\n");
        return 0;
     }
     if(signal(SIGUSR1, handler) == SIG_ERR)
     {
        printf("func signal err\n");
        return 0;
     }

     pid = fork();
     if(pid == -1)
     {
         perror("fork err\n");
         return 0;
     }

     if(pid == 0)
     {
        pid = getppid();
        kill(pid, SIGUSR1);//向老爹发信号
        //kill(0, SIGUSR1);//向同组的进程发信号
        exit(0);
     }

     int n = 3;

     do
     {
        printf("父进程开始睡眠\n");
        n = sleep(n);
        printf("父进程开始唤醒\n");
     }while(n > 0);

    printf("sleep函数执行完毕\n");
    return 0;
}


