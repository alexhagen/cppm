<<<<<<< HEAD
# cppm
cppm (C++ Minus) is a set of data structures that sit right in between the power of C++ and the economy of C
=======
# AH Timer Class
By Alex Hagen

The AH Timer class provides a linux supported C++ class for using interrupts to emit signals resonably accurately.  By using slots and connecting those slots to the signals emitted from the timer class, actions can be triggered at the interrupt priority level at times accurate to within several milliseconds.

## Contents
1. Installation
2. Usage
3. To Do

## 1. Installation
Installation of the AH Timer library is easy, simply download the contents of this library and unpack them (excluding the readme and tests) into the folder you're compiling out of.

## 2. Usage
Follow the steps below to use the AH Timer library.
* Lets say we have a class that needs to do something at a certain time.  For the sake of example, we'll use the following bomb:

```c++
// in file bomb.h
#include "ah_timer.h"
#include "sigslot.h"
#include <stdio.h>

class bomb : public sigslot::has_slots<>{
			// we must inherit slot properties from sigslot
public:
	/* functions                         */
	bomb(void); // our constructor
	void explode(void); // the actual bang
	void interval(void); // showing interval functionality
	/* objects                           */
	timer *t;
}

bomb::bomb(void){
	// we want to create a timer that will go off at a certain time
	t = new timer();
	// we now set the first fire to be in 3.5 seconds
	t->set(3.5);
	// we have to connect our bang to the timer
	t->sig->connect(this,&bomb::explode);
	// now we have to start the timer
	t->start();
}

void bomb:explode(void){
	// tell everybody that we exploded
	printf("BOOM!\n");
	// using built in diagnostics, we can see how much time it actually took
	printf("Bomb went off after %f seconds.\n",t->realtime);
}

void bomb::interval(void){
	// we can also set off our bomb every x seconds, here using 5.25
	t = new timer();
	// we have to set the interval instead of just the first instance
	t->set_int(5.25);
	// it's already connected so we don't have to do that, but we must start
	t->start();
}
```

* This class can be run using the following tester

```c++
// in file test_bomb.cpp
#include "bomb.h"
#include <stdio.h>
#include <unistd.h>

int main(void){
	// tell that we're running the bomb
	printf("Setting the bomb to go off after 3.5 seconds\n");
	// set off the timer and bomb
	bomb *b = new bomb();
	// wait until bomb has gone off
	sleep(4);
	// now switch tacts and do intervals
	printf("Setting the bomb to go off every 5.25 seconds\n");
	// use our function to make it go off
	b->interval();
	// wait till it goes off once
	sleep(6);
	// wait till it goes off again
	sleep(6)
	// end program
	return 0;
}
```

This can be compiled using (assuming that sigslot.h, bomb.h, and ah_timer.h are in the current directory):

```bash
$ g++ -std=c++11 test_bomb.cpp -o test_bomb
$ ./test_bomb
```

## 3. To Do
- [ ] Windows compatibility (added 10/12/14)
- [ ] Different start and interval times functionality (added 10/12/14)
- [ ] Better deconstructor (added 10/12/14)
>>>>>>> timer/master
