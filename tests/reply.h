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
