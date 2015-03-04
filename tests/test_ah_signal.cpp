#include "ah_signal.h"
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void action1(void){
	printf("signal 1 sounded!\n");
}

void action2(void){
	printf("signal 2 sounded!\n");
}

int main(void){
	printf("My process ID : %d\n", getpid());

	ah_signal* sig1 = new ah_signal(SIGALRM,action1);
	ah_signal* sig2 = new ah_signal(SIGHUP,action2);

while(1){
	sleep(1);
}

}