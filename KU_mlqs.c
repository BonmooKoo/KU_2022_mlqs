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
#define ITIMER ITIMER_REAL
//timer알람이 나오면
void alarm_handler (int signo){
    
}
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
    //timer create
    struct itimerval delay;
    int ret;

    signal(SIGALARM,alarm_handler);

     //timer setting
    delay.it_value.tv_sec=1;
    delay.it_value.tv_nsec=0;
    delay.it_interval.tv_sec=1;
    delay.it_interval.tv_sec=0;
    
    ret=setitimer(ITIMER,&delay,NULL);
    if(ret){
        perror("setitimer");
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

    //fork가 모두 생성되도록 기다려줌 이후 타이머 시작
    sleep(3);
    
    //fork 들 알파벳 순서로 linked list에 넣어줌

    


}