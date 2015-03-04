#ifndef ahsignal_h
#define ahsignal_h
#include <signal.h>

void signalHandler(int sig, siginfo_t *si, void *uc){
    timer *object = (timer *)si->si_value.sival_ptr;
    object->sig->emit();
}

class ah_signal {
public:
	ah_signal(void);
	void set(int);
	void action(void*);

private:
	struct sigevent e;
	struct sigaction sa;
	int signum;
};

ah_signal::ah_signal(void){
	sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = &signalHandler;
    sigemptyset(&sa.sa_mask);
}

ah_signal::ah_signal(int _signum,void* act){
	sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = &signalHandler;
    sigemptyset(&sa.sa_mask);
    set(_signum);
    action(act);
}

void ah_signal::set(int _signum){
	signum = _signum;
    // connecting the hardware signal SIGALRM to the static function
    // signalHandler, can't do much in signalHandler so we just emit
    // a signal
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = &signalHandler;
    sigemptyset(&sa.sa_mask);
    if (sigaction(signum, &sa, NULL) == -1)
    {
        return;
    }
    /* Set and enable alarm */
    e.sigev_notify = SIGEV_SIGNAL;
    e.sigev_signo = sigNo;
    e.sigev_value.sival_ptr = (void*) this;
}

void ah_signal::action(void* act){
    /* Set and enable alarm */
    e.sigev_notify = SIGEV_SIGNAL;
    e.sigev_signo = signum;
    e.sigev_value.sival_ptr = (void*) act;
}
#endif