#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

//wait 用于父进程等待并回收子进程资源

//如果父进程不调用 wait 方法， 则子进程退出后进程描述符仍然留在系统中，成为僵尸进程

//当父进程退出后，子进程还在运行，则子进程被 init 进程收养， 完成状态收集工作

//系统的进程号是有限的，大量僵尸进程会导致进程号不够分配
int main()
{
    pid_t fpid;

    int count = 0;

    fpid = fork();

    if (fpid < 0)
    {
        printf("fork error");
    }
    else if (fpid == 0)
    {
        printf("child process exit");
        exit(0);
    }
    else
    {
        pid_t waitPid;
        int status;
        waitPid = wait(&status);

        printf("status %d\n", status);
        printf("pid %d\n", fpid);
        printf("waitPid %d\n", waitPid);
    }

    return 0;
}