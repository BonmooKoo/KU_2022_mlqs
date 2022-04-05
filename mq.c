#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>


// linked list 로 queue 구현
typedef struct NODE
{
    struct NODE *next;
    int fork_id;
    int runtime;
    int alpha;

} NODE;

typedef struct QUEUE
{
    struct NODE *front;
    struct NODE *end;
    int count;

} QUEUE;
QUEUE* firstlv;
QUEUE* secondlv;
QUEUE* thirdlv;
int isEmpty(QUEUE *target)
{
    if (target->count == 0)
    {
        return 1;
    }
    else
        return 0;
}

void initqueue()
{
    firstlv=(QUEUE*)malloc(sizeof(QUEUE));
    firstlv->front = firstlv->end = NULL;
    firstlv->count = 0;
    secondlv=(QUEUE*)malloc(sizeof(QUEUE));
    secondlv->front = secondlv->end = NULL;
    secondlv->count = 0;
    thirdlv=(QUEUE*)malloc(sizeof(QUEUE));
    thirdlv->front = thirdlv->end = NULL;
    thirdlv->count = 0;
}
void enqueuenew(int fork_id, int alpha,QUEUE* target)
{
    NODE* new=(NODE*)malloc(sizeof(NODE));
    new->next = NULL;
    new->fork_id = fork_id;
    new->alpha = alpha;
    new->runtime = 0;
    if (target->count == 0)
    {
        target->front = new;
    }
    else
    {
        target->end->next = new;
    }
    target->end = new;
    target->count++;
/*
    NODE new;
    new.next=NULL;
    new.fork_id=fork_id;
    new.alpha=alpha;
    new.runtime=0;
    if(target->count=0){
        target->front=&new;
    }else{
        target->end->next=&new;
    }
    target->end=&new;
    target->count++;
*/
}

void enqueue(NODE *new, QUEUE *target)
{
    if (target->count == 0)
    {
        target->front = new;
    }
    else
    {
        target->end->next = new;
    }
    target->end = new;
    target->count++;
}

NODE* dequeue(QUEUE *target)
{
    NODE *now;  
    if (target->count == 0)
    {
        return NULL;
    }
    now = target->front;
    target->front = now->next;
    target->count--;
    
    return now;
}

// linked list 종료
// queue 선언


int main()
{
    printf("doing~~~~~~\n");
    initqueue();
    enqueuenew(1, 1,firstlv);
    // NODE* temp=dequeue(firstlv);
    // int j=temp->fork_id;
    // printf("%d",j);
    return 0;
}