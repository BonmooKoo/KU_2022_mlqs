#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <mqueue.h>
#include <pthread.h>

int main(int args,char* argv[]){
    const int a1 = atoi(argv[1]);
	const int a2 = atoi(argv[2]);
    const int num_thread = atoi(argv[3]);
    
}