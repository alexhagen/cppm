#pragma GCC diagnostic ignored "-Wwrite-strings"
#ifndef ah_tcp_client_h
#define ah_tcp_client_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <string>
#include "sigslot.h"

using namespace std;

#define BufferLength 100
#define DEFAULTPORT 9000

#define ERR_EMERG	0
#define ERR_CRIT	1
#define ERR_ERR		2
#define ERR_WARN	3
#define ERR_NOTICE	4
#define ERR_INFO	5
#define ERR_DEBUG	6

/*                            AH TCP Client class                             */
/*                             Author: Alex Hagen                             */
/* This client is a c implementation of the c directives to write to a tcp pipe
	and then wait for a response (or not).  I have tried to encapsulate and
	overload everything, and allow for connections of signals and slots to allow
	for advanced configuration.  */

class ah_tcp_client : public sigslot::has_slots<>{
public:
	int sd, rc, length = sizeof(int);
	struct sockaddr_in serveraddr;
	char buffer[BufferLength];
	char temp;
	int totalcnt = 0;
	struct hostent *hostp;
	sigslot::signal2<int,char*> error;
	sigslot::signal1<char*> data_ready;
	int port = DEFAULTPORT;
	unsigned long int ipAddr = INADDR_NONE;

	ah_tcp_client(void);
	void tcp_connect(char* _ipAddr, int _port);
	void disconnect(void);
	void send_data(char*);
};

ah_tcp_client::ah_tcp_client(void){
	// get a socket descriptor
	if((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		error.emit(ERR_CRIT,"socket not created.");
		exit(-1);
	} else {
		error.emit(ERR_DEBUG,"Socket created.");
	}

	// set up the server addresses using c commands
	memset(&serveraddr, 0x00, sizeof(struct sockaddr_in));
	serveraddr.sin_family = AF_INET;
}

void ah_tcp_client::tcp_connect(char* _ipAddr, int _port){
	// find the ipAddr from the string
	ipAddr = inet_addr(_ipAddr);
	// get the port
	port = _port;
	
	serveraddr.sin_port = htons(port);

	if((serveraddr.sin_addr.s_addr = ipAddr) == \
		(unsigned long)INADDR_NONE) {
		//
		hostp = gethostbyname(_ipAddr);
		if(hostp == (struct hostent *)NULL) {
			char err_str[255];
			sprintf(err_str,"HOST NOT FOUND --> ");
			sprintf(err_str,"h_errno = %d\n",h_errno);
			error.emit(ERR_ERR,err_str);
			close(sd);
			exit(-1);
		}
		memcpy(&serveraddr.sin_addr, hostp->h_addr, sizeof(serveraddr.sin_addr));
	}

	// connect() to server. */
	if((rc = connect(sd, (struct sockaddr *)&serveraddr, sizeof(serveraddr))) < 0) {
		//perror("Client-connect() error");
		error.emit(ERR_ERR,"couldn't connect.");
		close(sd);
		exit(-1);
	} else {
		error.emit(ERR_DEBUG,"connected");
	}

}

void ah_tcp_client::disconnect(void){
	close(sd);
}

void ah_tcp_client::send_data(char* msg){
	// write string to the server
	rc = write(sd, msg, strlen(msg));
	 
	if(rc < 0) {
		error.emit(ERR_ERR,"couldn't write to client");
		rc = getsockopt(sd, SOL_SOCKET, SO_ERROR, &temp, (socklen_t*)&length);
		if(rc == 0) {
			/* Print out the asynchronously received error. */
			errno = temp;
			error.emit(ERR_ERR,"Socket_option was");
		}
		close(sd);
		exit(-1);
	} else {
		char err_str[255];
		sprintf(err_str,"Client-write() is OK\n");
		sprintf(err_str,"String successfully sent lol!\n");
		error.emit(ERR_DEBUG,err_str);
	}
	
	totalcnt = 0;
	if (rc = recv(sd, &buffer,sizeof(buffer),0) <= 0){
		if(rc < 0) {
			error.emit(ERR_ERR,"client read error");
			close(sd);
			exit(-1);
		} else if (rc == 0) {
			error.emit(ERR_ERR,"Server program closed.");
			close(sd);
			exit(-1);
		}
	}
	error.emit(ERR_DEBUG,"Client-read() is OK");
	data_ready.emit(buffer);
}

#endif