#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
/*
int socket(int domain, int type, int protocol);
*/

/*
int bind(int sockfd, const struct sockaddr *addr,
                socklen_t addrlen);
*/

/*
 int listen(int sockfd, int backlog);
*/

/*
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
*/

void handle(int num)
{
	printf("recv num:%d \n", num);
	exit(0);
}

int main( )
{
     signal(SIGUSR1, handle);
    //创建socket
    int sockfd;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("fun socket\n");
        exit(0);
    }

    //定义socket结构体
    struct sockaddr_in srvaddr;
    //设置协议族
    srvaddr.sin_family = AF_INET;
    //设置端口
    srvaddr.sin_port = htons(8001);
    //增加ip地址
    srvaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); //127.0.0.1
    //srvaddr.sin_addr.s_addr = inet_addr(INADDR_ANY); //绑定本机的任意一个地址

    //客服端链接，这是长链接模型
    if (connect(sockfd, (struct sockaddr*) (&srvaddr), sizeof(srvaddr)) < 0)
    {
        perror("fun socket\n");
        exit(0);
    }


    pid_t pid = fork();
    if(pid > 0)//父进程向客户端发送从键盘接受的数据
    {
        //设置发送的缓存
        char sendbuf[1024] = { 0 };
        while (fgets(sendbuf, sizeof(sendbuf), stdin) != NULL)
        {
            write(sockfd, sendbuf, strlen(sendbuf)); //向服务器发送数据
            memset(sendbuf, 0, sizeof(sendbuf));
        }
    }
    else//子进程接受服务器端的数据并且打屏
    {
        //设置接收的缓存
        while(1)
        {
            char revbuf[1024] = { 0 };
            int ret = read(sockfd, revbuf, sizeof(revbuf));
            if (ret == 0)
            {
                //如果在读的过程中，对方已经关闭，tcpip协议会返回一个0数据包
                printf("服务器方已关闭\n");
                close(sockfd);
                kill(pid, SIGUSR1);
                exit(0);
            }
            else if (ret < 0)
            {
                perror("客户端读数据失败\n");
                close(sockfd);
                exit(0);
            }
            fputs(revbuf, stdout); //从服务器收到数据，打印屏幕
            memset(revbuf, 0, sizeof(revbuf));
        }
    }

    close(sockfd);
    return 0;
}

