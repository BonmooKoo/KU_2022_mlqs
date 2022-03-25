#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <mqueue.h>
#include <pthread.h>

#define CLOCKID CLOCK_PROCESS_CPUTIME_ID
int main(int args,char* argv[]){
    //초기 변수 설정
    if(args!=3){
        perror("lack of value");        
        return;
    }   
    pnum=atoi(argv[1]);
    if(pnum>26 || pnum<1){
        perror("wrong num of processes");
        return;
    }
    const int num_process = atoi(argv[1]);
	const int ts = atoi(argv[2]);
    
    //linked list 구현
    //3개 linked list 선입 선출 
    
    //맨 처음만 알파벳 순으로 정렬= 줄 

//timer 설정
    
    //CLOCKID = cpu time
    
    
    //필요한 변수들 
    int timer1;
    int timer_set1;
    struct sigevent evp;
    timer_t timerid;
    struct itimerspec delay;
    
    evp.sigev_value.sival_ptr=&timer;
    evp.sigev_notift=SIGEV_SIGNAL;
    evp.sigev_signo=SIGKILL;
    //evp : timer 만료시 (프로그램 종료 SIGKILL)
    //send signal evp.sigev_signo to process

    //timer create
    timer1=timer_create(CLOCKID,&evp,&timerid);
    if(timer1){
        perror("timer not created");
        return;
    }

    //timer setting
    delay.it_value.tv_sec=1;
    delay.it_value.tv_nsec=0;
    delay.it_interval.tv_sec=1;
    delay.it_interval.tv_sec=0;
    
    timer_set1=timer_settime(timerid,0,&delay,NULL);    
    if(timer_set1){
        perror("timer_set1");
        return;
    }


    //fork() 생성
    for (int i=0;i<num_process;i++){
        int fork_id=fork();
        if(fork_id==0){
            perror("wrong fork");
            return
        }
    }

    //fork가 모두 생성되도록 기다려줌
    sleep(3);

    //fork 들 알파벳 순서로 linked list에 넣어줌

    


}