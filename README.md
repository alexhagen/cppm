# AH TCP Server Class
By Alex Hagen

The AH TCP Server is a stater class to create functionality which a tcp server provides.  TCP servers are used as a connection based communication between processes, computers, etc.  They must first connect, and then they can accept data, and send a response.  Because they are connection based, there is some guarantee that the data will get there correctly (at the cost of speed), so you shouldn't need to do data checking (like you would with udp).

This class has been designed for subclassing.  This means you should make a subclass of public type `ah_tcp_server`.  Then, you should inherit and redeclare the `send_reply` method to determine what you want to do.  More detail is under the Usage section below.

## Contents
1. Installation
2. Usage
3. To Do

## 1. Installation
Installation of the ah_tcp_serv class is simple.  Place the ah_tcp_serv.h file somewhere where your compiler can see it (as well as the common directory, as it includes sigslot.h).  Then, subclass and move forward!

## 2. Usage
Follow the steps below to use the ah_tcp_serv class.
* Lets say we want to create a server that reads in a command integer and replies to that command integer a specific string.  First, we would need to subclass our ah_tcp_serv as below:

```c++
// in file reply.h
#include "ah_tcp_serv.h"


class reply : public ah_tcp_serv {
			  // we use public type here to inherit all of its members
public:
	// all we really need to do is to redeclare the send_reply member
	void send_reply(char*);
};

void reply::send_reply(char* buf) {
	// we need a container to construct our reply in
	char rply[1024];
	// we can determine what command came into the buffer using the input 
	// parameter buf
	if (atoi(buf) == 1){
		// if this was one, we want to make our rply variable reflect that
		sprintf(rply,"command 1 sent.\n");
	} else {
		// if this was something else, we need to reflect that, too
		sprintf(rply,"no recognizable command sent.\n");
	}
	// the previous code would be a perfect place to use a switch-case structure
	// since we're dealing with only equal comparisons

	// now we use the built in member send_msg to send that reply back to 
	// whatever client is connected
	send_msg(rply);
}
```

* This class can be run using the following tester

```c++
// in file test_reply.cpp
#include "reply.h"

int main(void){
	// create a new reply (the subclass of ah_tcp_serv) object and initialize it
	reply *server = new reply();
	// change our port to be 9000, which is high enough not to get in the way of
	// other programs
	server->change_port(9000);
	// start our server in blocking mode. The program will hang here until
	// forever.
	server->start_b();

	return 0;
}

```

This can be compiled and started using:

```bash
$ g++ -std=c++11 test_reply.cpp -I/dir/to/reply.h -I/dir/to/ah_tcp_serv.h \
> -I/dir/to/sigslot.h -o test_reply
$ ./test_reply
```

and tested from this or another system using

```bash
$ telnet 127.0.0.1 9000
> 1
> command 1 sent.
```

## 3. To Do
- [ ] Windows compatibility (added 10/15/14)
- [ ] Better testing telnet script in readme (added 10/15/14)
- [ ] Readme for the error signal catching (added 10/15/14)
- [ ] Non-Blocking server (added 10/15/14)
- [ ] Reply to many clients (added 10/15/14)
- [ ] 