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
int main()
{
    //注册一个信号
    //SIGINT 是ctrl+c会产生2号中断
    signal(SIGINT, handler);
    signal(SIGQUIT, handler);

    while(1)
    {
        pause();
    }
    printf("main...begin\n");

    return 0;
}

