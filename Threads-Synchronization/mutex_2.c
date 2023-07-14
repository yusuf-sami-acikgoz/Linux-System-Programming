/*
In the example below, different threads are synchronized using 
the mutex object while operating on the same linked list.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define MAX_ITEM 10000

typedef struct node_ts
{
    int val;
    struct node_ts* next;
}node_ts;

typedef struct llist_ts
{
    node_ts* head;
    node_ts* tail;
    int count;
    pthread_mutex_t mutex;
}llist_ts;

void create_linked_list(llist_ts* list);
void destroy_linked_list(llist_ts* list);
void print_linked_list(llist_ts* list);
void add_item_tail(llist_ts* list, int newVal);
void add_item_head(llist_ts* list, int newVal);

void *thread_proc1(void *param);
void *thread_proc2(void *param);
void *thread_proc3(void *param);
void exit_sys_errno(const char *msg, int errno);

llist_ts g_list;

void create_linked_list(llist_ts* list)
{
    int result;
    if((list->head = (node_ts*)malloc(sizeof(node_ts))) == NULL)
    {
        printf("Memory not allocated.\n");
        exit(EXIT_FAILURE);
    }

    list->tail      = list->head;
    list->count     = 1;
    list->head->val = 0;
    
    if ((result = pthread_mutex_init(&list->mutex, NULL)) != 0)
        exit_sys_errno("pthread_mutex_init", result);
}

void destroy_linked_list(llist_ts* list)
{
    node_ts* node_ptr;
    node_ts* tmp_ptr;
    node_ptr = list->head;

    for(int i = 0; i < list->count; ++i)
    {
        tmp_ptr = node_ptr->next;
        free(node_ptr);
        node_ptr = tmp_ptr;
    }
}

void add_item_head(llist_ts* list, int newVal)
{
    int result;
    node_ts* newNode;
    
    if((newNode = (node_ts*)malloc(sizeof(node_ts))) == NULL)
    {
        printf("Memory not allocated.\n");
        exit(EXIT_FAILURE);
    }
    newNode->val = newVal;

    if ((result = pthread_mutex_lock(&list->mutex)) != 0)
        exit_sys_errno("pthread_mutex_init", result);

    newNode->next = list->head;
    list->head = newNode;
    list->count++;

    if ((result = pthread_mutex_unlock(&list->mutex)) != 0)
        exit_sys_errno("pthread_mutex_init", result);
}

void add_item_tail(llist_ts* list, int newVal)
{
    int result;
    node_ts* newNode;
    
    if((newNode = (node_ts*)malloc(sizeof(node_ts))) == NULL)
    {
        printf("Memory not allocated.\n");
        exit(EXIT_FAILURE);
    }
    newNode->val = newVal;

    if ((result = pthread_mutex_lock(&list->mutex)) != 0)
        exit_sys_errno("pthread_mutex_init", result);

    list->tail->next = newNode;
    list->tail = newNode;
    list->count++;

    if ((result = pthread_mutex_unlock(&list->mutex)) != 0)
    exit_sys_errno("pthread_mutex_init", result);
}

void print_linked_list(llist_ts* list)
{
    node_ts* node_ptr = list->head;

    for(int i = 0; i < list->count; ++i)
    {
        printf("%d\n",node_ptr->val);
        node_ptr = node_ptr->next;
    }
}

void *thread_proc1(void *param)
{
    for(int i = 0; i < MAX_ITEM; ++i)
    {
        add_item_tail(&g_list, i);
    }
    return NULL;
}

void *thread_proc2(void *param)
{
    for(int i = 0; i < MAX_ITEM; ++i)
    {
        add_item_tail(&g_list, i);
    }
    return NULL;
}

void *thread_proc3(void *param)
{
    for(int i = 0; i < MAX_ITEM; ++i)
    {
        add_item_tail(&g_list, i);
    }
    return NULL;
}

int main()
{
    pthread_t tid1, tid2, tid3;
    int result;

    create_linked_list(&g_list);

    if ((result = pthread_create(&tid1, NULL, thread_proc1, NULL)) != 0)
        exit_sys_errno("pthread_create", result);
    
    if ((result = pthread_create(&tid2, NULL, thread_proc2, NULL)) != 0)
        exit_sys_errno("pthread_create", result);

    if ((result = pthread_create(&tid3, NULL, thread_proc3, NULL)) != 0)
        exit_sys_errno("pthread_create", result);

    if ((result = pthread_join(tid1, NULL)) != 0)
        exit_sys_errno("pthread_join", result);

    if ((result = pthread_join(tid2, NULL)) != 0)
        exit_sys_errno("pthread_join", result);

    if ((result = pthread_join(tid3, NULL)) != 0)
        exit_sys_errno("pthread_join", result);

    print_linked_list(&g_list);

    return 0;
}

void exit_sys_errno(const char *msg, int errno)
{
    fprintf(stderr, "%s:%s\n", msg, strerror(errno));
    exit(EXIT_FAILURE);
}
