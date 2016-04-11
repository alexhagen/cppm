#include <sys/time.h>
#include <signal.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>


class simple_timer{
public:
	simple_timer(void);
	simple_timer(int);
	void set(double);
	void start(void);
	void start(double);
	double stop(void);
	double get_t(void);
	double get_delt(void);
	double check_time(void);

private:
	void _start(void);
	double delt;
	double t;
	double start_t;
	int signum;
	timer_t timerID;
	struct timespec now;
    struct sigevent te;
    struct itimerspec its;
};

simple_timer::simple_timer(void){
	// if we start with the void constructor
	// we use the default sigalrm
	signum = 14;
};

simple_timer::simple_timer(int _signum){
	// if we start with a specified signal number
	// we have to pass the entered value into the
	// private value
	signum = _signum;
}

void simple_timer::set(double _delt){
	// pass the entered value into the private value
	delt = _delt;
}

void simple_timer::start(void){
	_start();
}

void simple_timer::start(double _delt){
	set(_delt);
	_start();
}

double simple_timer::stop(void){
	// first update the time
	check_time();
	// disconnect the signal/timer
	its.it_value.tv_sec = 0;
    its.it_value.tv_nsec = 0;
    its.it_interval.tv_sec = 0;
    its.it_interval.tv_nsec = 0;
    timer_settime(timerID,0,&its,NULL);
	// update the object t;
	return t;
}

void simple_timer::_start(void){
	int sec = floor(delt);
    int nsec = int((delt-floor(delt))*1.0E9);
    its.it_value.tv_sec = sec;
    its.it_value.tv_nsec = nsec;
    its.it_interval.tv_sec = 0;
    its.it_interval.tv_nsec = 0;
    te.sigev_notify = SIGEV_SIGNAL;
    te.sigev_signo = signum;
    timer_create(CLOCK_REALTIME, &te, &timerID);
    its.it_interval.tv_sec = 0;
    its.it_interval.tv_nsec = 0;
    timer_settime(timerID, 0, &its, NULL);
    clock_gettime(CLOCK_REALTIME, &now);
    start_t = double(now.tv_sec+(now.tv_nsec/1.0E9));
}

double simple_timer::check_time(void){
	clock_gettime(CLOCK_REALTIME, &now);
    t = double(now.tv_sec+(now.tv_nsec/1.0E9))-start_t;
		return t;
}

double simple_timer::get_t(void){
    return t;
}

double simple_timer::get_delt(void){
	return delt;
}
