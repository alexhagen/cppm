#ifndef ahsignal_h
#define ahsignal_h
#include <signal.h>

class ah_signal {
public:
	ah_signal(void);
	~ah_signal(void);
	void set(int);
	void action(void);

public:
	struct sigevent te;
	struct sigaction sa;
};

ah_signal::ah_signal(void){

}

ah_signal::~ah_signal(void){

}

void ah_signal::set(int signal){

}

void ah_signal::action(void){
	
}
#endif