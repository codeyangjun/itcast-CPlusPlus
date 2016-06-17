#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define ERR_EXIT(m) \
	do \
	{ \
		perror(m); \
		exit(EXIT_FAILURE); \
	} while(0)
/*
3、信号可靠信号不可靠信号相关
编程程序，要去实现如下功能：
	父进程创建子进程1和子进程2、子进程1向子进程2发送可靠信号，并传送额外数据为子进程1的pid*2;
	子进程2接受可靠信号的值，并发送给父进程，父进程把接受的值进行打印。
	提示：用sigqueue和sigaction实现
   */
int pidArray[10] = {0};

void handler(int num, siginfo_t *st, void *p)
{

    if(num == SIGRTMIN +1 )
    {
        //孩子1接收到了信号
        printf("孩子1收到信号，值:%d \n", st->si_value.sival_int);//父进程将子进程2的pid这个数据发给了，子进程1
        //pidArray[1] = st->si_value.sival_int;// 子进程1将从父进程收到的数据放在本全局区pidArray[1]中
       //
    }

    if(num == SIGRTMIN +2 )
    {
        //孩子2接收到了信号
        printf("孩子2收到信号，值:%d \n", st->si_value.sival_int);//子进程1将自己的2*pid值发给子进程2
        pidArray[3] = st->si_value.sival_int;//子进程2将子进程1发送的数据存到本全局区中
    }

    if (num == SIGRTMIN+3)
    {
        //父进程接收到了信号
        printf("父进程收到信号，值:%d \n", st->si_value.sival_int);//子进程2将自己收到的数据又发给父进程
        //printf("父进程中的pid[0]:%d pid[1]:%d \n", pidArray[0], pidArray[1]);//输出自己全局区中原来存的数据
        //pidArray[3] = st->si_value.sival_int;//父进程将从子进程2收到的数据存入本全局区中
    }
}

int main()
{
    struct sigaction act;
    act.sa_sigaction = handler;
    act.sa_flags =  SA_SIGINFO;

    if( sigaction(SIGRTMIN+1, &act, NULL) < 0 )
    {
        ERR_EXIT("注册SIGRTMIN+1信号失败");
    }

    if( sigaction(SIGRTMIN+2, &act, NULL) < 0 )
    {
        ERR_EXIT("注册SIGRTMIN+2信号失败");
    }

    if( sigaction(SIGRTMIN+3, &act, NULL) < 0 )
    {
        ERR_EXIT("注册SIGRTMIN+3信号失败");
    }

    //创建2个进程
    int i ;
    int pid;
    for(i = 0; i < 2; i++)
    {
        //父进程可以把所有的孩子ID缓存下来
        pidArray[i]  = pid = fork();
        if (pid == 0)
        {
            break; //如果是孩子，不参与fork
        }
        else if (pid > 0)
        {
             printf("父进程中的pid[0]:%d pid[1]:%d \n", pidArray[0], pidArray[1]);
        }
    }

    if(pid > 0)
    {
        printf("父进程运行\n");
        //父进程给进程1发送信号 把进程2的pid发过去
        //添加额外数据
        union sigval mysigval;
        mysigval.sival_int=pidArray[1];

        //发送消息
        sigqueue(pidArray[0],SIGRTMIN+1, mysigval);
    }

    //子进程1
    if (pid==0 && i==0)
    {
        printf("子进程1运行\n");
        printf("子进程1 sleep\n");
        sleep(2);
        printf("子进程1 sleep被打断以后，全局变量有值了\n");

        //添加额外数据
        union sigval mysigval;
        mysigval.sival_int=getpid()*2;

        //发送消息
        printf("子进程1给进程2发送信号getpid()*2:%d  \n", getpid()*2);
        sigqueue(pidArray[1],SIGRTMIN+2, mysigval);
        printf("子进程1中的pid[0]:%d pid[1]:%d \n", pidArray[0], pidArray[1]);
        exit(0);
    }

    //子进程2
    if (pid==0 && i==1)
    {
        printf("子进程2运行\n");
        printf("子进程2 sleep\n");
        sleep(3);
        printf("子进程2睡眠醒来，开始发信号\n");

        //添加额外数据
        union sigval mysigval;
        mysigval.sival_int=pidArray[3];

        //发送消息
        printf("子进程2给父进程发送信号pidArray[3]:%d  \n", pidArray[3]);
        sigqueue(getppid(),SIGRTMIN+3, mysigval);

        printf("子进程2中的pid[0]:%d pid[1]:%d \n", pidArray[0], pidArray[1]);
        exit(0);
    }

    sleep(4);
    int mypid = 0;
    while ( (mypid = waitpid(-1, NULL, 0)) > 0)
    {
        printf("子进程退出pid:%d \n", mypid);
    }

    printf("父进程退出\n");
    return 0;
}
