#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <mqueue.h>
#include <pthread.h>

int main(int args,char* argv[]){
    //초기 변수 설정
    if(args!=3){
        perror("lack of value");        
        exit(1);
    }   
    pnum=atoi(argv[1]);
    if(pnum>26 || pnum<1){
        perror("wrong num of processes");
        exit(1);
    }
    const int num_process = atoi(argv[1]);
	const int ts = atoi(argv[2]);
    
    //timer 설정
    int timer=timer_create(clock_t clockid,struct sigevent *restrict sevp, timer_t *restrict timerid);
    if(timer!=0){
        perror("timer not created");
        exit(1);
    }
    
//asdf
//i make perfect time table!

}