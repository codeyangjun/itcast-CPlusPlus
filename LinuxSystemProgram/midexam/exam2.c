#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>


#define ERR_EXIT(m) \
	do \
	{ \
		perror(m); \
		exit(EXIT_FAILURE); \
	} while(0)
/*
2、编程一个基本多进程测试框架，提示用户输入进程数、和每个进程数运行圈数。进行多进程压力测试。
要求父进程能监控所有子进程的退出，避免僵尸进程。
   */
void TestFunc(int loopnum)
{
    printf("loopnum:%d\n", loopnum+1);
}

int main()
{
    int pronum ;
    int loopnum ;

    printf("输入子进程个数:");
    scanf("%d", &pronum);
    printf("输入每子进程跑圈数:");
    scanf("%d", &loopnum);


    pid_t pid;
    int i, j;
    for(i = 0; i < pronum; i++)
    {
        pid = fork();
        if(pid == 0)
        {
            printf("子进程开始跑圈\n");
            for(j = 0; j < loopnum; j++)
            {
                TestFunc(j);
            }
            exit(0);//子进程退出了，不参与下次的fork
        }
    }

    //父进程能监控所有子进程的退出，避免僵尸进程。
    pid_t childpid ;
    while( (childpid = waitpid(-1, NULL, 0) )  >0 )
    {
        printf("子进程%d退出\n", childpid);
    }

    /*
    int res;
    while(1)
    {
        res = wait(NULL);
        if(res == -1)
        {
            if(errno==EINTR)//若阻塞中有别的信号
            {
                continue;
            }
            break;
        }
    }
    */

    printf("父进程退出\n");
    return 0;
}
