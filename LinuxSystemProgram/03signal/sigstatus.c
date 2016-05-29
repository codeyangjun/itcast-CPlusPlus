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
    else if(num == SIGUSR1)
    {
        printf("recv signal SIGUSR1\n");
    }
    else
    {
        printf("recv num:%d\n", num);
    }
}

void printsigset(sigset_t *set)
{
	int i;
	for (i=1; i<NSIG; ++i)
	{
		if (sigismember(set, i))
			putchar('1');
		else
			putchar('0');
	}
	printf("\n");
}

//信号异步通知事件
//如果我写了信号中断处理函数handler，当信号发生时，会自动调用我写的函数
int main()
{
     sigset_t pset; //用来打印的信号集
	sigset_t bset; //用来设置阻塞的信号集

	sigemptyset(&bset);
	sigaddset(&bset, SIGINT);

	if (signal(SIGINT, handler) == SIG_ERR)
		ERR_EXIT("signal error");

	if (signal(SIGQUIT, handler ) == SIG_ERR)
		ERR_EXIT("signal error");

	//读取或更改进程的信号屏蔽字 这里用来阻塞ctrl+c信号
	//ctrl+c信号被设置成阻塞，即使用户按下ctl+c键盘，也不会抵达
	sigprocmask(SIG_BLOCK, &bset, NULL);

	for (;;)
	{
		//获取未决 字信息

		sigpending(&pset);

		//打印信号未决  sigset_t字
		printsigset(&pset);
		sleep(1);
	}
	return 0;
}


