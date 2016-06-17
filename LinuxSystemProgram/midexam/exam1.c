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
信号相关试题
1、请编写一个程序，设置SIGINT和SIGQUIT信号，并在该程序中实现从文件中读取信息的操作，
并保证在读取文件且只有在读取文件的过程中不会被发送的SIGINT和SIGQUIT信号所打断。

   提示：在文件读取前阻塞信号，在文件读取后解除阻塞。
   */

void handler(int num, siginfo_t *info, void *p)
{

    if(num == SIGINT || num == SIGQUIT )
    {
        printf("%d信号抵达\n", num);
    }
    else
    {
        printf("其它信号：recv signal %d\n", num);
    }
}

int main()
{
    printf("开始阻塞信号\n");
    sigset_t bset; //用来设置阻塞的信号集
    sigemptyset(&bset);
    sigaddset(&bset, SIGINT);
    sigaddset(&bset, SIGQUIT);
    sigprocmask(SIG_BLOCK, &bset, NULL);
    printf("信号阻塞完毕\n");

    struct sigaction act;
    act.sa_sigaction = handler;
    act.sa_flags =  SA_SIGINFO;

    if( sigaction(SIGINT, &act, NULL) < 0 )
    {
        ERR_EXIT("注册SIGINT信号失败");
    }

    if( sigaction(SIGQUIT, &act, NULL) < 0 )
    {
        ERR_EXIT("注册SIGQUIT信号失败");
    }

    printf("开始读取文件\n");
    FILE * pFile;
    char buffer [1024];

    pFile = fopen ("data.txt" , "r");
    if (pFile == NULL)
        perror ("Error opening file");
    else
    {
        while ( ! feof (pFile) )
        {
            if ( fgets (buffer , 1024 , pFile) == NULL ) break;
            fputs (buffer , stdout);
        }
        fclose (pFile);
    }
    printf("文件读取完毕\n");

    printf("开始解除信号阻塞\n");
    sigemptyset(&bset);
    sigaddset(&bset, SIGINT);
    sigaddset(&bset, SIGQUIT);
    sigprocmask(SIG_UNBLOCK, &bset, NULL);
    printf("信号解除阻塞完毕\n");

    while(1)
    {
        pause();
    }

    return 0;
}
