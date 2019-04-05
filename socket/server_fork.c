
#include <sys/stat.h>

#include <fcntl.h>

#include <errno.h>

#include <netdb.h>

#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>

#include <netinet/in.h>

#include <arpa/inet.h>

#include <stdio.h>
#include <sys/wait.h>
#include <string.h>

#include <stdlib.h>

#include <unistd.h>

#define SERVER_PORT 6666

void hanldeClient();

int main()
{

    int serverSocket;

    struct sockaddr_in server_addr;

    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {

        perror("socket");

        return 1;
    }

    bzero(&server_addr, sizeof(server_addr));

    //初始化服务器端的套接字，并用htons和htonl将端口和地址转成网络字节序

    server_addr.sin_family = AF_INET;

    server_addr.sin_port = htons(SERVER_PORT);

    //ip可是是本服务器的ip，也可以用宏INADDR_ANY代替，代表0.0.0.0，表明所有地址

    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    //对于bind，accept之类的函数，里面套接字参数都是需要强制转换成(struct sockaddr *)

    //bind三个参数：服务器端的套接字的文件描述符，

    if (bind(serverSocket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {

        perror("bind");

        return 1;
    }

    //设置服务器上的socket为监听状态

    //第二个参数为连接队列，如果连接队列满了，则不接受新的请求， 客户端会出现WSAECONNREFUSED
    if (listen(serverSocket, 5) < 0)
    {

        perror("listen");

        return 1;
    }
    printf("监听端口: %d\n", SERVER_PORT);

    while (1)
    {

        struct sockaddr_in clientAddr;

        int addr_len = sizeof(clientAddr);

        int clientFd;

        clientFd = accept(serverSocket, (struct sockaddr *)&clientAddr, (socklen_t *)&addr_len);

        if (clientFd < 0)
        {
            perror("accept");
            continue;
        }

        pid_t pid;

        pid = fork();

        if (pid < 0)
        {
            perror("fork error");
            close(clientFd);
            continue;
        }
        else if (pid == 0)
        {
            //child process
            hanldeClient(clientAddr, clientFd);
            return 0;
        }
        else
        {
            //父进程
            continue;
        }
    }

    close(serverSocket);

    return 0;
}

void hanldeClient(struct sockaddr_in clientAddr, int clientFd)
{
    char buffer[200];
    int iDataNum;

    printf("IP is %s\n", inet_ntoa(clientAddr.sin_addr));

    printf("Port is %d\n", htons(clientAddr.sin_port));

    while (1)
    {

        printf("读取消息:");

        //参数1 socket 描述符
        //参数2 接受缓冲区
        //参数3 接受buf 的长度
        //参数4 不知道干嘛的 一般置为0
        iDataNum = recv(clientFd, buffer, 1024, 0);

        //内核为每个 socket 维护一个接受缓冲区与发送缓冲区(滑动窗口大小与拥塞控制的依据)

        //当接受缓冲区没有数据，连接又正常时，recv 会被阻塞住

        //recv 返回值 <0 出错 =0 连接关闭 >0 正常
        if (iDataNum < 0)
        {
            perror("recv null");
            break;
        }
        else if (iDataNum == 0)
        {
            printf("discount %d", clientFd);
            break;
        }

        buffer[iDataNum] = '\0';

        if (strcmp(buffer, "quit") == 0)
        {
            break;
        }

        printf("%s\n", buffer);

        //当发送缓冲已满，send 会被阻塞住
        char *response = "send from server";
        send(clientFd, response, strlen(response), 0);
    }

    close(clientFd);
}