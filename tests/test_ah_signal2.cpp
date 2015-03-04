#include "ah_signal.h"
#include <stdio.h>
#include <unistd.h>


void action1(int signum){
	printf("signal 1 sounded!\n");
}

void action2(int signum){
	printf("signal 2 sounded!\n");
}

int main(void){
	set_group("signal");
	ah_signal(SIGALRM,&action1);
	ah_signal(SIGHUP,&action2);

	send_signal(SIGALRM,"signal");

while(1){
	sleep(1);
}

}