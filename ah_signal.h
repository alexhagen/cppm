#ifndef ahsignal_h
#define ahsignal_h
#include <sys/time.h>
#include <signal.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <error.h>
#include <math.h>

class ah_signal {
public:
	ah_signal(void);
	ah_signal(int,void (*)());
	void set(int);
	void action(void (*)());
	void run(void);
	void (*act)() = NULL;

private:
	struct sigaction sa;
	int signum;
};

void *obj = NULL;

void signalHandler(int sig, siginfo_t *si, void *uc){
    ah_signal *object = (ah_signal *)obj;
    object->act();
}

ah_signal::ah_signal(void){
	sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = &signalHandler;
    sigemptyset(&sa.sa_mask);
}

ah_signal::ah_signal(int _signum,void (*_act)()){
	sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = &signalHandler;
    sigemptyset(&sa.sa_mask);
    set(_signum);
    action(_act);
}

void ah_signal::set(int _signum){
	signum = _signum;
    // connecting the hardware signal SIGALRM to the static function
    // signalHandler, can't do much in signalHandler so we just emit
    // a signal
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = &signalHandler;
    obj = (void*) this;
    sigemptyset(&sa.sa_mask);
    if (sigaction(signum, &sa, NULL) == -1)
    {
        return;
    }
}

void ah_signal::action(void (*_act)()){
    act = _act;
}

void ah_signal::run(void){
	printf("signal was run!");
	act();
}

#endif