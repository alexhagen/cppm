# AH_Value: An Object Oriented Class for Tracking Variable Values and Updating on Variable Change

By Alex Hagen

The AH Value class is a simple, encapsulated c++ class to help track values.  It is written with the signal-slot paradigm so that events can be triggered as the values change, corresponding.  This is a way to decrease the overhead that must happen to trace some values (such as checking if something is over a certain value every loop).

## Contents
1. Installation
2. Usage
3. To Do

## 1. Installation
Installation of the ah_value class is simple.  Place the ah_value.h file somewhere where your compiler can see it (as well as the common directory, as it includes sigslot.h).  Then, connect your signals and move forward.

## 2. Usage
Follow the steps below to use the `ah_value` class.

* Lets say we want to create a program that indicates whether a motor is up to speed, and thereafter, how much it has varied from that speed.  We would write a notifier class, as below.

```c++

// in file notifier.h
#include "sigslot.h"
#include "ah_value.h"
#include <stdio.h>

class notifier : public sigslot::has_slots<>{
				 // we have to make this a sigslot so that it can accept slots
public:
	// a value object for our notifier class
	value<int> *vl;
	// a constructor to pass in an external notifier class
	notifier(value<int>*);
	// we need a slot that will tell when it's up to speed
	void up_to_speed(void);
}

notifier::notifier(value<int> *_vl){
	vl = _vl;
}

void notifier::up_to_speed(void){
	// let us know that we're up to speed
	printf("Motor is up to speed\n");
	// now reset the max min so we only get the maximum and minimum when up to 
	// speed
	vl->resetmaxmin();
}
```

* This can be run using the following tester

```c++

// in file test_value.cpp
#include "ah_value.h"
#include "notifier.h"

int main(void){
	// we need to create a new value class of type int
	value<int> *vl = new value<int>();
	// we need to create a new notifier class and pass in our value
	notifier *ntfr = new notifier(vl);
	// we need to connect the signal for above level to the slot telling us
	// we're up to speed
	vl->abovelevel.connect(&ntfr,notifier::up_to_speed);
	// now we have to set what level we're up to speed at
	vl->setlevel(5);
	// now we want to slowly increase the speed
	vl->set(0);
	vl->set(1);
	vl->set(3);
	vl->set(5);
	vl->set(7);
	vl->set(9);
	vl->set(7);
	vl->set(5);
	// now that we're back below speed (remember we said *above* 5), we can end 
	// and look at details
	printf("The maximum when up to speed was %d, and the minimum was %d.\n", \
		vl->getmax(),vl->getmin());

	return 0;
}
```

This can be compiled and run using:

```bash

$ g++ -std=c++11 test_value.cpp -I/dir/to/notifier.h -I/dir/to/ah_value.h \
> -I/dir/to/sigslot.h -o test_value
$ ./test_value
```

## 3. To Do
- [ ] Absolute value support (added 10/16/14)
- [ ] "Smart" change, indicating if value was changed and is still at speed (added 10/16/14)