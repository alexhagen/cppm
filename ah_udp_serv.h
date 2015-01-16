#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include "sigslot.h"

#define PORT 6000
#define GROUP "239.1.1.1"

class ah_udp_serv : public sigslot::has_slots<>{
public:
	ah_udp_serv(void);
	void send(int);
private:
	struct sockaddr_in addr;
	int addrlen, sock, cnt;
	struct ip_mreq mreq;
};

ah_udp_serv::ah_udp_serv(void){
	// setup socket
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) {
		perror("socket");
		exit(1);}
	bzero((char *)&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(PORT);
	addrlen = sizeof(addr);
	// setup group
	addr.sin_addr.s_addr = inet_addr(GROUP);
}

void ah_udp_serv::send(int msg){
	char message[50];
	sprintf(message, "%d",msg);
   	printf("sending: %s\n", message);
	cnt = sendto(sock, message, sizeof(message), 0,
		(struct sockaddr *) &addr, addrlen);
	if (cnt < 0) {
		perror("sendto");
	exit(1);
	}
}