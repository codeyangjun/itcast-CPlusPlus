#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
void handler(int num)
{
    printf("recv num:%d\n", num);
    if(num == SIGQUIT)
    {
        exit(0);
    }
}
//信号异步通知事件
//如果我写了信号中断处理函数handler，当信号发生时，会自动调用我写的函数
int main1()
{
    //注册一个信号
    //SIGINT 是ctrl+c会产生2号中断
    char temchar;
    signal(SIGINT, handler);

    printf("如果你键入a字符，那么将恢复SIGINT默认行为\n");
    while( (temchar = getchar()) != 'a')
    {
        pause();
    }
    signal(SIGINT, SIG_DFL);

    while(1)
    {
        pause();
    }
    printf("main...end\n");

    return 0;
}

//信号异步通知事件
//如果我写了信号中断处理函数handler，当信号发生时，会自动调用我写的函数
int main()
{
    //注册一个信号
    //SIGINT 是ctrl+c会产生2号中断
    char temchar;
    __sighandler_t oldHandler = signal(SIGINT, handler);

    printf("如果你键入a字符，那么将恢复SIGINT默认行为\n");
    while( (temchar = getchar())  !=  'a')
    {
        pause();
    }
    signal(SIGINT, oldHandler);

    while(1)
    {
        pause();
    }
    printf("main...end\n");

    return 0;
}


