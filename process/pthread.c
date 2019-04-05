#include <pthread.h>
#include <stdio.h>

#define NUM_THREADS 5 //线程个数

void *say_hello(void *args)
{
    printf("Hello Runoob！\n");
    pthread_exit(NULL); //线程主动退出时调用的函数，线程即可以显示退出，也可以隐示退出
}

int main()
{
    //定义线程的 id 变量，多个变量使用数组
    pthread_t tids[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; ++i)
    {
        //参数依次是：创建的线程id，线程参数，调用的函数，传入的函数参数
        int ret = pthread_create(&tids[i], NULL, say_hello, NULL);
        if (ret != 0)
        {
            printf("pthread_create error: error_code = %d\n", ret);
        }
    }

    //等各个线程退出后，进程才结束，否则进程强制结束了，线程可能还没反应过来；
    //pthread_join 可以用来回收线程资源
    pthread_join(tids[0], NULL);
    pthread_join(tids[1], NULL);
    pthread_join(tids[2], NULL);
    pthread_join(tids[3], NULL);
    pthread_join(tids[4], NULL);
}
