#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bufqueue.h"

void queue_insert(struct queue_class * bufqueue ,void * ptr){
    struct listhead *head=&bufqueue->listhead;
    struct listhead *node=GET_QUEUE_NODE(ptr);
    head->next->prev=node;
    node->next=head->next;
    head->next=node;
    node->prev=head;
    bufqueue->n_free++;
}

//insure bufqueue->nfree >0
void *queue_get(struct queue_class * bufqueue){
    struct listhead *head=&bufqueue->listhead;
    struct listhead *node;
    node=head->prev;
    head->prev->prev->next=head;
    head->prev=head->prev->prev;
    bufqueue->n_free--;
    node->prev=NULL;
    node->next=NULL;
    return GET_MEM(node);
}


static inline void  init_queue_class(struct queue_class * ini_ptr,uint64_t size){
    ini_ptr->size=size;
    ini_ptr->n_free=0;
    ini_ptr->listhead.prev=&ini_ptr->listhead;
    ini_ptr->listhead.next=&ini_ptr->listhead;
}

void bufqueue_init(struct queue_class **ptr){
    *ptr=(struct queue_class*)malloc(MAX_ORDER* sizeof(struct queue_class));
    init_queue_class(*ptr,BASE_SIZE);
    for(int i=0;i<MAX_ORDER-1;i++){
        init_queue_class(*ptr+i+1,(*ptr+i)->size*FACTOR);
    }
}


//
void * bufqueue_allocate(struct queue_class * bufqueue,uint64_t size){
    void *p;
    //size is too large,malloc directly
    if(size>MAX_SIZE){
        p=malloc(size);
        return p;
    }
    //serch in bufqueue
    int i=0;
    while(bufqueue[i].size<size) i++;
    if(bufqueue[i].n_free>0){
        p=queue_get(bufqueue+i);

        //alloc
    }else{
        p=malloc(bufqueue[i].size+2* sizeof(POINTER));
        ((struct listhead *)p)->prev=NULL;
        ((struct listhead *)p)->next=NULL;
        return (POINTER)p+2;
    }

}

void bufqueue_free(struct queue_class * bufqueue,void * ptr,uint64_t size){
    if(size>MAX_SIZE){
        free(ptr);
    }
    int i=0;
    while(bufqueue[i].size<size) i++;
    queue_insert(bufqueue+i,ptr);
}

void dump(struct queue_class *bufqueue){
    printf("\n-------------------\n");
    for(int i=0;i<MAX_ORDER;i++){
        printf("%lu %lu %lu\n",i,bufqueue[i].size,bufqueue[i].n_free);
    }
}

