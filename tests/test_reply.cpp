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
