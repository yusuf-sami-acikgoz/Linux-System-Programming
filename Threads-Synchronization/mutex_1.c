/*
In the example below, the value of the global "g_count" 
object is wanted to be increased by two separate threads. 
The synchronization of these threads operating on the same 
object is provided with the "g_mutex" object. The correct 
result, 2000000, is obtained.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

void *thread_proc1(void *param);
void *thread_proc2(void *param);
void exit_sys_errno(const char *msg, int errno);

pthread_mutex_t g_mutex;
int g_count = 0;

int main()
{
    pthread_t tid1, tid2;
    int result;

    if ((result = pthread_mutex_init(&g_mutex, NULL)) != 0)
        exit_sys_errno("pthread_mutex_init", result);

    if ((result = pthread_create(&tid1, NULL, thread_proc1, NULL)) != 0)
        exit_sys_errno("pthread_create", result);
    
    if ((result = pthread_create(&tid2, NULL, thread_proc2, NULL)) != 0)
        exit_sys_errno("pthread_create", result);

    if ((result = pthread_join(tid1, NULL)) != 0)
        exit_sys_errno("pthread_join", result);

    if ((result = pthread_join(tid2, NULL)) != 0)
        exit_sys_errno("pthread_join", result);

    printf("COUNTER: %d\n", g_count);

    if ((result = pthread_mutex_destroy(&g_mutex)) != 0)
        exit_sys_errno("pthread_mutex_destroy", result);

    return 0;
}

void *thread_proc1(void *param)
{
    int result;
    for(int i = 0; i < 1000000; ++i)
    {
        if ((result = pthread_mutex_lock(&g_mutex)) != 0)
            exit_sys_errno("pthread_mutex_init", result);

        ++g_count;    

        if ((result = pthread_mutex_unlock(&g_mutex)) != 0)
            exit_sys_errno("pthread_mutex_init", result);
    }

    return NULL;
}

void *thread_proc2(void *param)
{
    int result;

    for(int i = 0; i < 1000000; ++i)
    {
        if ((result = pthread_mutex_lock(&g_mutex)) != 0)
            exit_sys_errno("pthread_mutex_init", result);

        ++g_count;    

        if ((result = pthread_mutex_unlock(&g_mutex)) != 0)
            exit_sys_errno("pthread_mutex_init", result);
    }

    return NULL;
}

void exit_sys_errno(const char *msg, int errno)
{
    fprintf(stderr, "%s:%s\n", msg, strerror(errno));

    exit(EXIT_FAILURE);
}

