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
#define EVERYSEC SIGUSR1
#define EVERYTEN SIGUSR2
void sec_handler(int signo){
    //다른 애로 넘겨줌
}
void ten_handler(int signo){
    //priority 초기화
}
int main(int args,char* argv[]){
//초기 입력 변수 처리
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

//signal hander 정의
    struct sigaction sa1;
    struct sigaction sa2;
    sa1.sa_hander=sec_handler;
    sa2.sa_hander=ten_handler;
    
    int sig1=sigaction(EVERYSEC,&sa1,NULL);
    if(sig1){
        perror("sig1");
    }
    int sig2=sigaction(EVERYTEN,&sa2,NULL);
    if(sig2){
        perror("sig2");
    }

//timer 설정
    
    //CLOCKID = cpu time
    
    
    //필요한 변수들 
    int timer1;
    int timer_set1;
    struct sigevent evp;
    timer_t timerid;
    struct itimerspec delay1;
    
    evp.sigev_value.sival_ptr=&timer;
    evp.sigev_notift=SIGEV_SIGNAL;
    evp.sigev_signo=EVERYSEC;
    //evp : timer 만료시
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
    
    



    //fork() 생성
    for (int i=0;i<num_process;i++){
        int fork_id=fork();
        if(fork_id==0){
            perror("wrong fork");
            return
        }
    }

    //fork가 모두 생성되도록 기다려줌 이후 타이머 시작
    sleep(3);
    timer_set1=timer_settime(timerid,0,&delay1,NULL);    
    if(timer_set1){
        perror("timer_set1");
        return;
    }
    //fork 들 알파벳 순서로 linked list에 넣어줌

    


}