#include <unistd.h>
#include "ah_udp_client.h"

class alert : public sigslot::has_slots<>{
public:
	alert(void);
	void msg_recv(char*);
};

alert::alert(void){
	printf("Started the alert.\n");
}

void alert::msg_recv(char* msg){
	printf("the message was: %s\n",msg);
}

int main(void) {
	ah_udp_client mcast;
	alert *alrt = new alert();
	mcast.recv.connect(alrt,&alert::msg_recv);
	mcast.start();
	sleep(5);
}