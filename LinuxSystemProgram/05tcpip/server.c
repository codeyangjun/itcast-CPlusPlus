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

/*
 int setsockopt(int sockfd, int level, int optname,const void *optval, socklen_t optlen);
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
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

/*
 int setsockopt(int sockfd, int level, int optname,const void *optval, socklen_t optlen);
*/

int main( )
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

    int optval = 1;
    //设置地址可以复用 ，关键点： 通过setsockopt 设置 第三个参数 为 SO_REUSEADDR，
    //起作用也只是服务端退出后，还在等待状态可以使用地址复用。
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval))
            < 0)
    {
        perror("setsockopt bind\n");
        exit(0);
    }


    //绑定IP
    if (bind(sockfd, (struct sockaddr *) &srvaddr, sizeof(srvaddr)) < 0)
    {
        perror("fun bind\n");
        exit(0);
    }

    //一但调用listen函数，这个套接字sockfd将变成被动套接字;只能接受连接，不能主动的发送连接
    //listen 做了两个队列。。。。。。
    // 队列由内核管理，一部分是完成三次握手的，一部分是没有完成三次握手的。
    if (listen(sockfd, SOMAXCONN) < 0)
    {
        perror("fun listen\n");
        exit(0);
    }

    //struct sockaddr peeraddr;  //通用ip
    //socklen_t perrlen ;
    struct sockaddr_in peeraddr; //tcpip地址结构
    socklen_t peerlen = sizeof(peeraddr);
    int conn = 0;

    while(1)
    {
        //accept接受已经完成三次握手的链接，没有链接会阻塞直到有链接
        conn = accept(sockfd, (struct sockaddr *) &peeraddr, (socklen_t *) &peerlen);//每来一个新的链接
        if (conn == -1)
        {
            perror("fun listen\n");
            exit(0);
        }

        printf("perradd:%s\n perrport:%d\n", inet_ntoa(peeraddr.sin_addr), ntohs(peeraddr.sin_port));

        int pid = fork();//每来一个新的链接，服务器端启动一个新的进程服务
        if(pid == 0)
        {
            close(sockfd); //子进程不需要侦听，
            char revbuf[1024] = { 0 };
            while (1)
            {
                int ret = read(conn, revbuf, sizeof(revbuf));
                if (ret == 0)
                {
                    //如果在读的过程中，对方已经关闭，tcpip协议会返回一个0数据包
                    printf("对方已关闭\n");
                    close(conn);
                    exit(0);
                }
                else if (ret < 0)
                {

                    perror("读数据失败\n");
                    exit(0);
                }
                //ssize_t write(int fd, const void *buf, size_t count);
                fputs(revbuf, stdout); //服务器端收到数据，打印屏幕
                write(conn, revbuf, ret); //服务器端向客户端回发报文
            }
        }
        else if(pid > 0)
        {
            close(conn);//父进程关闭连接
        }
        else
        {
            close(conn);
            close(sockfd);
            exit(0);
        }
    }

    close(sockfd);
    return 0;
}
