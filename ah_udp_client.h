#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "sigslot.h"

#define PORT 6000
#define GROUP "239.1.1.1"

#define BUFSIZE 2048

class ah_udp_client : public sigslot::has_slots<>{
public:
	ah_udp_client(void);
    void start(void);
    sigslot::signal1<char*> data_ready;
private:
    struct sockaddr_in addr;
    socklen_t addrlen;
    int sock, cnt;
    struct ip_mreq mreq;
    char message[BUFSIZE];
    bool cont;
};

ah_udp_client::ah_udp_client(void){
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket");
        exit(1);
    }
    bzero((char *)&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(PORT);
    addrlen = sizeof(addr);

    if (bind(sock, (struct sockaddr *) &addr, sizeof(addr)) < 0) {        
        perror("bind");
        exit(1);
    }    
    mreq.imr_multiaddr.s_addr = inet_addr(GROUP);         
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);         
    if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP,
        &mreq, sizeof(mreq)) < 0) {
        perror("setsockopt mreq");
        exit(1);
    }         
}

void ah_udp_client::start(void) {
    while (cont) {
        cnt = recvfrom(sock, message, sizeof(message), 0, 
            (struct sockaddr *) &addr, &addrlen);
        if (cnt < 0) {
            perror("recvfrom");
            exit(1);
        } else if (cnt == 0) {
            break;
        }
            printf("%s: message = \"%s\"\n", inet_ntoa(addr.sin_addr), message);
            data_ready.emit(message);
    }
};