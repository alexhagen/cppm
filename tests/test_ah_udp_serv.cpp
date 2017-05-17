#include "ah_udp_serv.h"


int main(void) {
	ah_udp_serv *mcast = new ah_udp_serv();
	while (1){
		mcast->send(1);
		sleep(3);
	}
}