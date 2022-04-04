#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <mqueue.h>

#define CLOCKID CLOCK_MONOTONIC
#define EVERYSEC SIGUSR1
#define ENDPROCESS SIGUSR2

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
    NODE *front;
    NODE *end;
    int count;

} QUEUE;

int isEmpty(struct QUEUE *target)
{
    if (target->count = 0)
    {
        return 1;
    }
    else
        return 0;
}
int peek(struct QUEUE *target)
{
    if (target->count != 0)
    {
        return target->front->fork_id;
    }
    else
        return 0;
}
void initqueue(struct QUEUE *target)
{
    target->front = target->end = NULL;
    target->count = 0;
}
void enqueuenew(int fork_id, int alpha, struct QUEUE *target)
{
    struct NODE *new = malloc(sizeof(struct NODE));
    new->next = NULL;
    new->fork_id = fork_id;
    new->alpha = alpha;
    new->runtime = 0;
    if (target->count = 0)
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
/*
void enqueue(int fork_id,int runtime,struct QUEUE *target){
    struct NODE *new=malloc(sizeof(struct NODE));
    new->next=NULL;
    new->fork_id=fork_id;
    new->runtime=runtime;
    if(target->count=0){
        target->front=new;
    }
    else{
        target->end->next=new;
    }
    target->end=new;
    target->count++;
}
*/
void enqueue(struct NODE *new, struct QUEUE *target)
{
    if (target->count = 0)
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

NODE *dequeue(struct QUEUE *target)
{
    NODE *now;
    if (target->count = 0)
    {
        return NULL;
    }
    now = target->front;
    target->front = now->next;
    target->count--;
    // free(now);
    return now;
}
/*
void sortQueue(struct QUEUE *target)
{
    int counter=target->count;
    int max=100;
    for(int i=0;i<counter;i++){
        
    }
}
*/
// linked list 종료
// queue 선언
QUEUE firstlv;
QUEUE secondlv;
QUEUE thirdlv;
//

// signal hander 정의

int timercount = 0;
int pnum = 0;
int totaltimeslice = 0;
void sec_handler(int signo)
{
    // handler 불릴때마다 count 증가
    timercount++;
    if (timercount == totaltimeslice)
    {
        //프로그램 종료
        pid_t pid = getpid();
        kill(pid, SIGTERM);
        // or
        // exit(0);
    }
    if (timercount % 10 == 0)
    {
        NODE *temp;
        //초기화(전부 first로 넣어줌)
        while (secondlv.count != 0)
        {
            temp = dequeue(&secondlv);
            enqueue(&temp, &firstlv);
        }
        while (thirdlv.count != 0)
        {
            temp = dequeue(&secondlv);
            enqueue(&temp, &firstlv);
        }
        //정렬!
    }
    NODE *old;
    if (!isEmpty(&firstlv))
    {
        //기본 실행 fork 종료
        //새 fork 실행
        old = dequeue(&firstlv);
        old->runtime++;
        if (old->runtime == 2)
        {
            enqueue(&old, &secondlv);
            old->runtime = 0;
        }
        else
        {
            enqueue(&old, &firstlv);
        }
    }
    else if (!isEmpty(&secondlv))
    {
        old = dequeue(&secondlv);
        old->runtime++;
        if (old->runtime == 2)
        {
            enqueue(&old, &thirdlv);
            old->runtime = 0;
        }
        else
        {
            enqueue(&old, &secondlv);
        }
    }
    else
    {
        old = dequeue(&thirdlv);
        old->runtime++;
        if (old->runtime == 2)
        {
            enqueue(&old, &thirdlv);
            old->runtime = 0;
        }
        else
        {
            enqueue(&old, &thirdlv);
        }
    }
    kill(old->fork_id, SIGSTOP);
    if (firstlv.count != 0)
    {
        kill(firstlv.front->fork_id, SIGCONT);
    }
    else if (secondlv.count != 0)
    {
        kill(secondlv.front->fork_id, SIGCONT);
    }
    else if (thirdlv.count != 0)
    {
        kill(thirdlv.front->fork_id, SIGCONT);
    }
}
void end_handler(int signo)
{
    // priority 초기화
}
// signal handler 종료

int main(int args, char *argv[])
{
    //초기 입력 변수 처리
    if (args != 3)
    {
        perror("lack of value");
        return;
    }
    pnum = atoi(argv[1]);
    if (pnum > 26 || pnum < 1)
    {
        perror("wrong num of processes");
        return;
    }
    totaltimeslice = atoi(argv[2]);

    // linked list 구현
    // 3개 linked list 선입 선출

    //초기화
    initqueue(&firstlv);
    initqueue(&secondlv);
    initqueue(&thirdlv);

    // signal hander 정의
    /*
    struct sigaction sa1;
    struct sigaction sa2;
    sa1.sa_handler=sec_handler(&firstlv,&secondlv,&thirdlv,pnum);
    //sa2.sa_handler=end_handler;

    int sig1=sigaction(EVERYSEC,&sa1,NULL);
    if(sig1){
        perror("sig1");
    }
    int sig2=sigaction(ENDPROCESS,&sa2,NULL);
    if(sig2){
        perror("sig2");
    }*/
    signal(EVERYSEC, sec_handler);
    // signal hander 끝

    // fork() 생성
    char input[2];
    input[1] = '\0';
    for (int i = 0; i < pnum; i++)
    {
        int fork_id = fork();
        if (fork_id == -1)
        {
            perror("wrong fork");
            return;
        }
        else if (fork_id == 0)
        {
            // fork 프로그램 실행
            input[1] = 'A' + i;
            execl('./ku_app', 'ku_app', &input, NULL);
        }
        // parent
        else
            enqueuenew(fork_id, i, &firstlv);
    }
    // fork 끝
    // fork가 모두 생성되도록 기다려줌 이후 타이머 시작
    sleep(3);

    // timer 설정
    // CLOCKID = real time
    //필요한 변수들
    struct itimerval delay;
    int ret;

    signal(SIGALRM, sec_handler);

    // timer setting
    delay.it_value.tv_sec = 1;
    delay.it_value.tv_usec = 0;
    delay.it_interval.tv_sec = 1;
    delay.it_interval.tv_usec = 0;

    ret = setitimer(ITIMER_REAL, &delay, NULL);
    if (ret)
    {
        perror("setitimer");
        return;
    }

    // dequeue lv1 ('A')
    NODE *temp = firstlv.front;
    kill(temp->fork_id, SIGCONT);

    while(1){

    }
    //이후 sig handler 에 맡김
    //종료
}