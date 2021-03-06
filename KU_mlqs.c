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
QUEUE *firstlv;
QUEUE *secondlv;
QUEUE *thirdlv;

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
    firstlv = (QUEUE *)malloc(sizeof(QUEUE));
    firstlv->front = firstlv->end = NULL;
    firstlv->count = 0;
    secondlv = (QUEUE *)malloc(sizeof(QUEUE));
    secondlv->front = secondlv->end = NULL;
    secondlv->count = 0;
    thirdlv = (QUEUE *)malloc(sizeof(QUEUE));
    thirdlv->front = thirdlv->end = NULL;
    thirdlv->count = 0;
}
void enqueuenew(int fork_id, int alpha, QUEUE *target)
{
    NODE *new = (NODE *)malloc(sizeof(NODE));
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

NODE *dequeue(QUEUE *target)
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

void sortQueue(struct QUEUE *target)
{
    
    QUEUE *tempQue;
    tempQue = (QUEUE *)malloc(sizeof(QUEUE));
    tempQue->front = tempQue->end = NULL;
    tempQue->count = 0;
    int counter = target->count;
        
    //현재 노드
    NODE *now;
    //최소 node
    NODE *pointer;
    for (int j = 0; j < counter; j++)
    {
        now = target->front;
        int min = 100;
    // 최소 알파벳을 찾고
        for (int i = 0; i < counter; i++)
        {
            if (now->alpha < min)
            {
                min = now->alpha;
                pointer = now;
            }
            now = now->next;
        }
        //NODE 상태로 넣는다.
        enqueue(pointer, tempQue);
    }
    target=tempQue;
}


// linked list 종료

// queue 선언
QUEUE *firstlv;
QUEUE *secondlv;
QUEUE *thirdlv;
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
        if (firstlv->count != 0)
        {
            kill(firstlv->front->fork_id, SIGTERM);
        }
        else if (secondlv->count != 0)
        {
            kill(secondlv->front->fork_id, SIGTERM);
        }
        else if (thirdlv->count != 0)
        {
            kill(thirdlv->front->fork_id, SIGTERM);
        }
        pid_t pid = getpid();
        kill(pid, SIGTERM);
        // or
        // exit(0);
    }

    NODE *old;
    if (firstlv->count > 0)
    {
        //기본 실행 fork 종료
        //새 fork 실행
        old = dequeue(firstlv);
        old->runtime++;
        if (old->runtime == 2)
        {
            enqueue(old, secondlv);
            old->runtime = 0;
        }
        else
        {
            enqueue(old, firstlv);
        }
    }
    else if (secondlv->count > 0)
    {
        old = dequeue(secondlv);
        old->runtime++;
        if (old->runtime == 2)
        {
            enqueue(old, thirdlv);
            old->runtime = 0;
        }
        else
        {
            enqueue(old, secondlv);
        }
    }
    else
    {
        old = dequeue(thirdlv);
        old->runtime++;
        if (old->runtime == 2)
        {
            enqueue(old, thirdlv);
            old->runtime = 0;
        }
        else
        {
            enqueue(old, thirdlv);
        }
    }
    
    kill(old->fork_id, SIGSTOP);
    if (timercount % 10 == 0)
    {
        NODE *temp;
        //초기화(전부 first로 넣어줌)
        
        
        int seoondcount = secondlv->count;
        for (int i = 0; i < seoondcount; i++)
        {
            temp = dequeue(secondlv);
            temp->runtime=0;
            enqueue(temp, thirdlv);
        }
        sortQueue(secondlv);
        int thirdcount = thirdlv->count;
        for (int i = 0; i < thirdcount; i++)
        {
            temp = dequeue(thirdlv);
            temp->runtime=0;
            enqueue(temp, secondlv);
        }
    }
    
    if (firstlv->count != 0)
    {
        kill(firstlv->front->fork_id, SIGCONT);
    }
    else if (secondlv->count != 0)
    {
        kill(secondlv->front->fork_id, SIGCONT);
    }
    else if (thirdlv->count != 0)
    {
        kill(thirdlv->front->fork_id, SIGCONT);
    }
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
    // 알파벳 시간
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
    initqueue();

    // fork() 생성
    char input[2] = {'A', '\0'};
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
            input[0] = 'A' + i;
            execl("./ku_app", "ku_app", input, NULL);
        }
        // parent
        else
        {
            enqueuenew(fork_id, i, firstlv);
        }
    }
    // fork 끝
    // fork가 모두 생성되도록 기다려줌 이후 타이머 시작
    sleep(3);

    // timer 설정
    // CLOCKID = real time
    //필요한 변수들
    struct itimerval delay;
    int ret;

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
    NODE *temp = firstlv->front;
    kill(temp->fork_id, SIGCONT);

    //이후 sig handler 에 맡김
    signal(SIGALRM, sec_handler);

    //종료
    while (1)
    {
    }
}
