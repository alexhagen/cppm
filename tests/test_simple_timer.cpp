#include <stdio.h>
#include "simple_timer.h"
#include "ah_signal.h"

bool cont = true;

void action1(int signum){
	printf("action 1!\n");
	cont = false;
}

int main(void){
	simple_timer* timer = new simple_timer();
	ah_signal(SIGALRM,&action1);
	timer->start(10.0);
	while(cont){

	}
	printf("Time was %f\n",timer->get_t());

	return 0;
}