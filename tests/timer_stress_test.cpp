#include "timer.h"
#include "sigslot.h"
#include <stdio.h>
#include <thread>

class timers : public sigslot::has_slots<>{
public:
	timer *t1,*t2,*t3;
	timers();
	void timer1(void);
	void timer2(void);
	void timer3(void);
	void print_timer1(void);
	void print_timer2(void);
	void print_timer3(void);
};

timers::timers(void){
	std::thread t1(&timers::timer1,this);
	std::thread t2(&timers::timer2,this);
	std::thread t3(&timers::timer3,this);

	t1.join();
	t2.join();
	t3.join();
}

void timers::timer1(void){
	t1 = new timer();
	t1->sig->connect(this,&timers::print_timer1);
	t1->set_int(1.00);
	t1->start();
}
void timers::timer2(void){
	t2 = new timer();
	t2->sig->connect(this,&timers::print_timer2);
	t2->set_int(2.00);
	t2->start();
}
void timers::timer3(void){
	t3 = new timer();
	t3->sig->connect(this,&timers::print_timer3);
	t3->set_int(3.00);
	t3->start();
}

void timers::print_timer1(void){
	printf("[TIMER1]: completed after %f seconds.\n",t1->realtime);
}
void timers::print_timer2(void){
	printf("[TIMER2]: completed after %f seconds.\n",t2->realtime);
}
void timers::print_timer3(void){
	printf("[TIMER3]: completed after %f seconds.\n",t3->realtime);
}

int main(void){
	timers *t = new timers();
	for(;;){
		// do nothing
	}
	return 0;
}