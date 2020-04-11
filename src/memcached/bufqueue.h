#ifndef __BUFQUEUE__
#define __BUFQUEUE__
#include <math.h>
#define uint64_t unsigned long


//The size of the smallest buffer
#define BASE_SIZE 100
//The growth rate
#define FACTOR  1.5
//Maximum number of increases
#define MAX_ORDER 41

#define MAX_SIZE BASE_SIZE*pow(FACTOR,MAX_ORDER-1)

#define POINTER void**

#define GET_QUEUE_NODE(p) \
            (struct listhead *)((POINTER)p-2)
#define GET_MEM(listnode) \
            (void *)(listnode+1)

struct listhead{
    struct listhead * prev,* next;
};

struct queue_class{
    uint64_t size;
    uint64_t n_free;
    struct listhead listhead;
};


void *queue_get(struct queue_class * bufqueue);
void queue_insert(struct queue_class *bufqueue,void *ptr);

void bufqueue_init(struct queue_class **ptr);
void * bufqueue_allocate(struct queue_class * bufqueue,uint64_t size);
void bufqueue_free(struct queue_class * bufqueue,void * ptr,uint64_t size);

void dump(struct queue_class *bufqueue);

#endif