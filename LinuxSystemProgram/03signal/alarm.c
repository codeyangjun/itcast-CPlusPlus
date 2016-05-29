#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
void handler(int num)
{
    printf("recv signal %d\n", num);
    alarm(1);
}

//信号异步通知事件
//如果我写了信号中断处理函数handler，当信号发生时，会自动调用我写的函数
int main()
{
     printf("main...begin\n");

     if(signal(SIGALRM, handler) == SIG_ERR)
     {
        printf("func signal err\n");
        return 0;
     }

    alarm(1);
    while(1)
    {
        pause();
        printf("pause return\n");
    }
    return 0;
}


