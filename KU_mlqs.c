#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <mqueue.h>

#define CLOCKID CLOCK_MONOTONIC
#define EVERYSEC SIGUSR1
#define ENDPROCESS SIGUSR2


//linked list 로 queue 구현

typedef struct NODE{
    struct NODE *next;
    int fork_id;
    int runtime;
}NODE;
typedef struct QUEUE{
    NODE *front;
    NODE *end;
    int count;//
    
}QUEUE;
int isEmpty(struct QUEUE *target){
    if (target->count=0){
        return 1;
    }else return 0;
}
int peek(struct QUEUE *target){
    if(target->count!=0){
        return target->front->fork_id;
    }else return 0;
}
void initqueue(struct QUEUE *target){
    target->front=target->end=NULL;
    target->count=0;
}
void enqueuenew(int fork_id,struct QUEUE *target){
    struct NODE *new=malloc(sizeof(struct NODE));
    new->next=NULL;
    new->fork_id=fork_id;
    new->runtime=0;
    if(target->count=0){
        target->front=new;
    }
    else{
        target->end->next=new;
    }
    target->end=new;
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
void enqueue(struct NODE *new,struct QUEUE *target){
    if(target->count=0){
        target->front=new;
    }
    else{
        target->end->next=new;
    }
    target->end=new;
    target->count++;
}

NODE* dequeue(struct QUEUE *target){
    NODE *now;
    if(target->count=0){
        return NULL;
    }
    now=target->front;
    target->front=now->next;
    target->count--;
    //free(now);
    return now;
}
//linked list 종료

int timercount=0;
int totaltimeslice=0;
//signal hander 정의
void sec_handler(struct QUEUE *first,struct QUEUE *second,struct QUEUE *third,int pnum){
    //handler 불릴때마다 count 증가
    timercount++;
    totaltimeslice++;
    if(totaltimeslice==pnum){
        //프로그램 종료
        pid_t pid=getpid();
        kill(pid,SIGTERM);
        //or
        //exit(0);
    }
    if(timercount==10){
        NODE* temp;
        //초기화(전부 first로 넣어줌)
        while(second->count!=0){
            temp=dequeue(&second);
            enqueue(&temp,&first);
        }
        while(third->count!=0){
            temp=dequeue(&second);
            enqueue(&temp,&first);
        }
        //정렬!

        timercount=0;
    }
    
    if(!isEmpty(&first)){
        //기본 실행 fork 종료
        //새 fork 실행
        NODE* temp=dequeue(&first);
        temp->runtime++;
        if(temp->runtime==2){
            enqueue(&temp,&second);
            
        }else {
            enqueue(&temp,&first);
        }
        temp->runtime=0;

        kill(temp->fork_id,SIGSTOP);
        if(first->count!=0){
            kill(first->front->fork_id,SIGCONT);
        }
    }
    else if(!isEmpty(&second)){
        NODE* temp=dequeue(&second);
        temp->runtime++;
        if(temp->runtime==2){
            enqueue(&temp,&third);
            
        }else {
            enqueue(&temp,&second);
        }
        temp->runtime=0;
        kill(temp->fork_id,SIGSTOP);
        if(second->count!=0){
            kill(second->front->fork_id,SIGCONT);
        }
    }
    else{
        NODE* temp=dequeue(&third);
        temp->runtime++;
        if(temp->runtime==2){
            enqueue(&temp,&third);
            
        }else {
            enqueue(&temp,&third);
        }
        temp->runtime=0;

        kill(temp->fork_id,SIGSTOP);
        if(third->count!=0){
            kill(third->front->fork_id,SIGCONT);
        }
    }
}
void end_handler(int signo){
    //priority 초기화

}
//signal handler 종료


int main(int args,char* argv[]){
//초기 입력 변수 처리
    if(args!=3){
        perror("lack of value");        
        return;
    }   
    int pnum=atoi(argv[1]);
    if(pnum>26 || pnum<1){
        perror("wrong num of processes");
        return;
    }
    const int num_process = atoi(argv[1]);
	const int ts = atoi(argv[2]);
    
//linked list 구현
    //3개 linked list 선입 선출 
    QUEUE firstlv;
    QUEUE secondlv;
    QUEUE thirdlv;
    //초기화
    initqueue(&firstlv);
    initqueue(&secondlv);
    initqueue(&thirdlv);
        
//signal hander 정의
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
    }
//signal hander 끝

    //fork() 생성
    char input='A';
    for (int i=0;i<num_process;i++){
        int fork_id=fork();
        if(fork_id==-1){    
            perror("wrong fork");
            return;
        }
        else if(fork_id==0){
            //fork 프로그램 실행
            execl('./ku_app','ku_app',input,NULL);
            input+=1;
        }
        //parent 
        else enqueuenew(fork_id,&firstlv);
    }
    //fork 끝
    //fork가 모두 생성되도록 기다려줌 이후 타이머 시작
    sleep(3);

//timer 설정
    //CLOCKID = real time    
    //필요한 변수들 
    int timer1; // timer1은 매초 변환
    int timer_set1;
    int timer2; // timer2은 X초 이후 process 종료
    int timer_set2;
    
    timer_t timerid;
    struct itimerspec delay1;
    
    struct sigevent evp;
    evp.sigev_value.sival_ptr=&timer1;
    evp.sigev_notify=SIGEV_SIGNAL;
    evp.sigev_signo=EVERYSEC;
    //evp : timer 만료시 (1초마다)
    //send signal evp.sigev_signo to process

    //timer create
    //1초마다 EVERYSEC 신호를 전달하는 친구
    timer1=timer_create(CLOCKID,&evp,&timerid);
    if(timer1){
        perror("timer not created");
        return;
    }

    //timer setting
    delay1.it_value.tv_sec=1;
    delay1.it_value.tv_nsec=0;
    delay1.it_interval.tv_sec=1;
    delay1.it_interval.tv_sec=0;
    

    timer_set1=timer_settime(timerid,0,&delay1,NULL);    
    if(timer_set1){
        perror("timer_set1");
        return;
    }
    
    //dequeue lv1 ('A')
    NODE* temp=dequeue(&firstlv);
    kill(temp->fork_id,SIGCONT);

    //이후 sig handler 에 맡김
    //종료
}