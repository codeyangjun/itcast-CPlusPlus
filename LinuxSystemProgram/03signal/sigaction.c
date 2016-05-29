#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>

#define ERR_EXIT(m) \
	do \
	{ \
		perror(m); \
		exit(EXIT_FAILURE); \
	} while(0)

void handler(int num)
{
    if(num == SIGINT)
    {
        printf("recv signal SIGINT\n");
    }
    else
    {
        printf("recv num:%d\n", num);
    }
}

//信号异步通知事件A
//如果我写了信号中断处理函数handler，当信号发生时，会自动调用我写的函数
int main()
{
    struct sigaction act;
    act.sa_handler = handler;

    sigemptyset(&act.sa_mask);
	sigaddset(&act.sa_mask, SIGQUIT);
	act.sa_flags = 0;

    sigaction(SIGINT, &act, NULL);
	for (;;)
	{
		sleep(1);
	}
	return 0;
}


