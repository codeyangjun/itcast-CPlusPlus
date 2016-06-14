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
/*
 The  sigaction() system call is used to change the action taken by a process on receipt of
       a specific signal.  (See signal(7) for an overview of signals.)

       signum specifies the signal and can be any valid signal except SIGKILL and SIGSTOP.

       If act is non-NULL, the new action for signal signum is installed from act.  If oldact  is
       non-NULL, the previous action is saved in oldact.

       The sigaction structure is defined as something like:

           struct sigaction {
               void     (*sa_handler)(int);
               void     (*sa_sigaction)(int, siginfo_t *, void *);
               sigset_t   sa_mask;
               int        sa_flags;
               void     (*sa_restorer)(void);
           };
*/

void handler(int num, siginfo_t *info, void *p)
{
    if(num == SIGINT )
    {
        int value = info->si_value.sival_int;
        printf("接受到不可靠信号:%d,接受值:%d\n", num, value);

    }
    else if(num == SIGRTMIN)
    {
        int value = info->si_value.sival_int;
        printf("接受到可靠信号:%d,接受值:%d\n", num, value);

    }
    else if(num == SIGUSR1)
    {
        printf("接受到用户自定义信号：%d,开始解除阻塞\n", num);
        sigset_t bset;
        sigemptyset(&bset);
        sigaddset(&bset, SIGINT);
        sigaddset(&bset, SIGRTMIN);
        sigprocmask(SIG_UNBLOCK, &bset, NULL);
        printf("解除阻塞完毕\n");
    }
    else
    {
        printf("其它信号：recv signal %d\n", num);
    }
}

int main()
{
    struct sigaction act;
    act.sa_sigaction = handler;
    act.sa_flags =  SA_SIGINFO;

    //把SIGINT SIGRTMIN信号加到本进程的阻塞状态中
     sigset_t bset;
     sigemptyset(&bset);
     sigaddset(&bset, SIGINT);
     sigaddset(&bset, SIGRTMIN);
     sigprocmask(SIG_BLOCK, &bset, NULL);

     //注册不可靠信号SIGIN
    if( sigaction(SIGINT, &act, NULL) < 0 )
    {
        ERR_EXIT("注册不可靠信号失败");
    }
     //注册可靠信号SIGRTMIN
    if( sigaction(SIGRTMIN, &act, NULL) < 0 )
    {
        ERR_EXIT("注册可靠信号失败");
    }
     //注册自定义信号SIGUSR1
     if( sigaction(SIGUSR1, &act, NULL) < 0 )
     {
        ERR_EXIT("注册自定义信号失败");
     }

     pid_t pid;
     pid = fork();

     //子进程给父进程发送3次不可靠信号,3次可靠信号，然后发送SIGUSR1信号接触阻塞
     if(pid == 0)
     {
        union sigval v;
        v.sival_int = 201;
        int ret ;

        int i;
        for(i = 0; i < 3; i++)
        {
            ret = sigqueue(getppid(), SIGINT, v);
            if(ret != 0)
            {
                printf("发送不可靠信号失败\n");
                exit(0);
            }
        }

        v.sival_int = 301;
        for(i = 0; i < 3; i++)
        {
            ret = sigqueue(getppid(), SIGRTMIN, v);
            if(ret != 0)
            {
                printf("发送可靠信号失败\n");
                exit(0);
            }
        }
         //然后发送SIGUSR1信号接触阻塞
        kill(getppid(), SIGUSR1);
     }
     else
     {
        while(1)
        {
            sleep(1);
        }
     }
    return 0;
}


