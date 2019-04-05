#include <stdio.h>
#include <unistd.h>

//fork出去以后父子进程内存页共享

//当子进程修改完变量以后，按理说是要 copy 一份新的内存页的， 但是程序地址是偏移地址，所以输出来的 count 是一致的

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
        //子进程
        printf("child process print\n");
        printf("child count address %p\n", &count);
        count++;
    }
    else
    {
        //父进程
        printf("father process print\n");
        printf("father count address %p\n", &count);
        count++;
        count++;
    }
    sleep(1);
    printf("count value %d\n", count);
    printf("count address after add %p\n", &count);

    return 0;
}