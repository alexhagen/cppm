#ifndef timer_h
#define timer_h

#include "sigslot.h"
#include <sys/time.h>
#include <signal.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <error.h>
#include <math.h>




class timer : public sigslot::has_slots<>{
  public:
    timer(void);
    ~timer(void);
    void set(double time);
    void set_int(double time);
    void on_deck(void);
    void start(void);
    void clear(void);
    void finish(void);
    void stop(void);
    double delt;
    double realtime;
    timer_t timerID;

    sigslot::signal0<>* sig;



  private:
    //static void signalHandler(int sig, siginfo_t *si, void *uc);
    // real timing functions
    double start_time;
    struct timespec now;

    struct sigevent         te;
    struct itimerspec       its;
    struct sigaction        sa;
    int                     sigNo = SIGRTMIN;
};

void signalHandler(int sig, siginfo_t *si, void *uc){
    timer *object = (timer *)si->si_value.sival_ptr;
    object->sig->emit();
}


timer::timer(){
    sig = new sigslot::signal0<>;
    // connecting the hardware signal SIGALRM to the static function
    // signalHandler, can't do much in signalHandler so we just emit
    // a signal
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = &signalHandler;
    sigemptyset(&sa.sa_mask);
    if (sigaction(sigNo, &sa, NULL) == -1)
    {
        return;
    }
    /* Set and enable alarm */
    te.sigev_notify = SIGEV_SIGNAL;
    te.sigev_signo = sigNo;
    te.sigev_value.sival_ptr = (void*) this;
    timer_create(CLOCK_REALTIME, &te, &timerID);
    its.it_interval.tv_sec = 0;
    its.it_interval.tv_nsec = 0;
    //printf("timer id is: %d\n",timerID);
}

timer::~timer(){
    sig->disconnect_all();
    timerID = NULL;
    clear();
}

void timer::set(double t){
    delt = t;
    sig->connect(this,&timer::finish);
    int sec = floor(delt);
    int nsec = int((delt-floor(delt))*1.0E9);
    its.it_value.tv_sec = sec;
    its.it_value.tv_nsec = nsec;
}

void timer::set_int(double t){
    delt = t;
    sig->connect(this,&timer::finish);
    int sec = floor(delt);
    int nsec = int((delt-floor(delt))*1.0E9);
    its.it_value.tv_sec = sec;
    its.it_value.tv_nsec = nsec;
    its.it_interval.tv_sec = sec;
    its.it_interval.tv_nsec = nsec;
}

void timer::start(){
    timer_settime(timerID, 0, &its, NULL);
    clock_gettime(CLOCK_REALTIME, &now);
    start_time = double(now.tv_sec+(now.tv_nsec/1.0E9));
}

void timer::finish(){
    clock_gettime(CLOCK_REALTIME, &now);
    realtime = double(now.tv_sec+(now.tv_nsec/1.0E9))-start_time;
    if (realtime > 10000) realtime = 0;
    printf("ran finish\n");
}

void timer::clear(){
    its.it_value.tv_sec = 0;
    its.it_value.tv_nsec = 0;
    timer_settime(timerID,0,&its,NULL);
    printf("ran clear\n");
    delete this;
}

void timer::stop(){
    its.it_value.tv_sec = 0;
    its.it_value.tv_nsec = 0;
    timer_settime(timerID,0,&its,NULL);
}

#endif
