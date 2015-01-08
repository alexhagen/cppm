# AH TCP Server Class
By Alex Hagen

The AH TCP Client class is a starter class which will help to connect to a tcp server.  This can be used from my ah_tcp_serv class, or any other tcp server (such as a qt server).  Because this is tcp, it has some guarantee that the data will be delivered.

This class is designed to use signals and slots, using the wonderful sigslot.h library.  You can connect a slot to the data_ready signal and do actions when you read in a full data.  This has been inspired by qt and the sigslot library.

## Contents
1. Installation
2. Usage
3. To Do

## 1. Installation
Installation of the ah_tcp_client class is simple.  Place the ah_tcp_client.h file somewhere where your compiler can see it (as well as the common directory, as it includes sigslot.h).  Then, connect your signals and move forward

## 2. Usage
Follow the steps below to use the ah_tcp_client class.
* Lets say we want to create a tcp client that will connect to a server, send a message, and then display the response.  We would write the following class to display the reply.

```c++
// in file response.h
#include "sigslot.h"
#include <stdio.h>

class response : public sigslot::has_slots<>{
				 // we have to make this a sigslot so that it can accept slots
public:
	// we need a slot that will print out the response from the server
	void printdata(char*);
}

void response::printdata(char* str){
	// print out the data so we can make sure we got a response
	printf("[DATA]: %s\n",str);
}
```

* This class and the client can be run using the following tester

```c++
// in file test_client.cpp
#include "ah_tcp_client.h"
#include "response.h"

int main(void){
	// create an object of ah_tcp_client type and initalize it
	ah_tcp_client *client = new ah_tcp_client();
	// create an object to print out our response
	response *rspns = new response();
	// connect the data_ready signal to the printdata slot
	client->data_ready.connect(rspns,&response::printdata);
	// connect the client to the server
	client->tcp_connect("127.0.0.1",9000);
	// send the integer 1
	client->send_data("1");
	// send something else that wont be recognized
	client->send_data("none");
	// disconnect
	client->disconnect();

	return 0;
}

```

This can be compiled and started using:

```bash
$ g++ -std=c++11 test_client.cpp -I/dir/to/response.h -I/dir/to/ah_tcp_client.h \
> -I/dir/to/sigslot.h -o test_client
$ ./test_client
```

when this is run after a server (see ah_tcp_serv instructions) is running, we should get some replies from the server.

## 3. To Do
- [ ] Windows compatibility (added 10/15/14)
- [ ] Better compatibility between the ah_tcp_client and ah_tcp_serv readme (added 10/15/14)
- [ ] Send_int, send_byte, send_bin and other functions (added 10/15/14)Updated
