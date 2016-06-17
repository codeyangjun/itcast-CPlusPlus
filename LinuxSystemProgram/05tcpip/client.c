#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>
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

int main1( )
{
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
    //设置接受和发送的缓存。
    char revbuf[1024] = { 0 };
    char sendbuf[1024] = { 0 };
    //
    //char *fgets(char *s, int size, FILE *stream); 从stream 读取size-1大小的数据存入s,最后加'\0'
    while (fgets(sendbuf, sizeof(sendbuf), stdin) != NULL)
    {
        //向服务写数据
        //ssize_t write(int fd, const void *buf, size_t count);
        // 从buf中读取count大小的数据存入文件描述符为fd的文件中。
        write(sockfd, sendbuf, strlen(sendbuf)); //向服务器发送数据
        //从服务器读数据
        //ssize_t read(int fd, void *buf, size_t count);
        //从文件描述符为fd的文件中读取大小为count的数据存入buf中。
        read(sockfd, revbuf, sizeof(revbuf));
        fputs(revbuf, stdout); //从服务器收到数据，打印屏幕
        memset(revbuf, 0, sizeof(revbuf));
        memset(sendbuf, 0, sizeof(sendbuf));
    }
    close(sockfd);
    return 0;
}

int main( )
{

    //客服端链接，这是短链接模型

    for (int i = 0; i < 10; i++)
    {
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

        int conn = connect(sockfd, (struct sockaddr*) (&srvaddr), sizeof(srvaddr)) ;
        if (conn < 0)
        {
            perror("fun socket\n");
            exit(0);
        }
        //设置接受和发送的缓存。
        char revbuf[1024] = { 0 };
        char sendbuf[1024] = { 0 };
        //
        //char *fgets(char *s, int size, FILE *stream); 从stream 读取size-1大小的数据存入s,最后加'\0'
        sprintf(sendbuf, "i=%d\n", i);
        //向服务写数据
        //ssize_t write(int fd, const void *buf, size_t count);
        // 从buf中读取count大小的数据存入文件描述符为fd的文件中。
        write(sockfd, sendbuf, strlen(sendbuf)); //向服务器发送数据
        //从服务器读数据
        //ssize_t read(int fd, void *buf, size_t count);
        //从文件描述符为fd的文件中读取大小为count的数据存入buf中。
        read(sockfd, revbuf, sizeof(revbuf));
        fputs(revbuf, stdout); //从服务器收到数据，打印屏幕
        memset(revbuf, 0, sizeof(revbuf));
        memset(sendbuf, 0, sizeof(sendbuf));

        close(sockfd);
    }

    return 0;
}
