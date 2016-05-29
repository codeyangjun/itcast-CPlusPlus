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

void handler(int signum, siginfo_t *s_t, void *p)
{
        int myint = 0;
		printf("recv signum : %d \n", signum);
		myint = s_t->si_value.sival_int;
		printf("%d %d \n", myint, s_t->si_int );
}


int main()
{
    struct sigaction act;
    act.sa_sigaction = handler;
    //如果父进程的T回调函数， 准备接受额外数据
	act.sa_flags = SA_SIGINFO;

    sigaction(SIGINT, &act, NULL);

    pid_t pid = fork();
    if (pid == -1)
	{
		printf("fork err...\n");
		return 0;
	}

    //子进程向父进程间通过 sigqueue 发送数据
	if (pid == 0)
	{
        int i = 0;
        union sigval  mysigval;
	   //mysigval.sival_ptr = (void *)&t;
	   mysigval.sival_int = 222;

	   int ret;
	   for (i=0; i<10; i++)
		{
			ret  = sigqueue(getppid(), SIGINT, mysigval);
			if (ret != 0)
			{
				printf("sigqueue .....\n");
				exit(0);
			}
			else
			{
				printf("sigqueue...successs\n");
				sleep(2);
			}
		}
	}
    else if(pid > 0)
    {
        for (;;)
        {
            pause();
        }
    }

	return 0;
}


