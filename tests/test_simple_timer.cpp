#include <stdio.h>
#include "simple_timer.h"
#include "ah_signal.h"

bool cont = true;
simple_timer* timer;

void action1(int signum){
	timer->stop();
	printf("action 1!\n");
	cont = false;
}

int main(void){
	timer = new simple_timer();
	ah_signal(SIGALRM,&action1);
	timer->start(1.5);
	while(cont){

	}
	printf("Time was %f\n",timer->get_t());

	return 0;
}