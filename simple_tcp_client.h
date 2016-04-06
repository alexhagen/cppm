#pragma GCC diagnostic ignored "-Wwrite-strings"
#ifndef simple_tcp_client_h
#define simple_tcp_client_h

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


/* Simple TCP Client class.
 *
 * `simple_tcp_client` implements a tcp client which encapsulates all of the
 * `c` directives to create a tcp client which writes to a server and then
 * can be programmed to respond or not.
 */
class simple_tcp_client {
public:
	int sd, rc, length = sizeof(int);
	struct sockaddr_in serveraddr;
	char buffer[BufferLength];
	char temp;
	int totalcnt = 0;
	bool data_ready_sig = false;
	struct hostent *hostp;
	char err_str[255],data[BufferLength];
	int port = DEFAULTPORT;
	unsigned long int ipAddr = INADDR_NONE;

	simple_tcp_client(void);
	void tcp_connect(char* _ipAddr, int _port);
	void disconnect(void);
	void send_data(char*);
	void accumulate(int);
	void accumulate(char);
};

simple_tcp_client::simple_tcp_client(void){
	// get a socket descriptor
	if((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		//error.emit(ERR_CRIT,"socket not created.");
		exit(-1);
	} else {
		//error.emit(ERR_DEBUG,"Socket created.");
	}

	// set up the server addresses using c commands
	memset(&serveraddr, 0x00, sizeof(struct sockaddr_in));
	serveraddr.sin_family = AF_INET;
}

void simple_tcp_client::tcp_connect(char* _ipAddr, int _port){
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
			//error.emit(ERR_ERR,err_str);
			close(sd);
			exit(-1);
		}
		memcpy(&serveraddr.sin_addr, hostp->h_addr, sizeof(serveraddr.sin_addr));
	}

	// connect() to server. */
	if((rc = connect(sd, (struct sockaddr *)&serveraddr, sizeof(serveraddr))) < 0) {
		//perror("Client-connect() error");
		//error.emit(ERR_ERR,"couldn't connect.");
		close(sd);
		exit(-1);
	} else {
		//error.emit(ERR_DEBUG,"connected");
	}

}

void simple_tcp_client::disconnect(void){
	close(sd);
}

void simple_tcp_client::accumulate(int num){
	memset(buffer,0,sizeof(buffer));
	rc = recv(sd, &buffer,sizeof(buffer),0);
	while (rc < num){
		rc = recv(sd, &buffer,sizeof(buffer),0);
		printf("waiting...\n");
	}
	memcpy(this->data,buffer,num*sizeof(this->data[0]));
	this->data_ready_sig = true;
}

void simple_tcp_client::accumulate(char start_byte){
	char byte = 0;
	memset(buffer,0,sizeof(buffer));
	rc = recv(sd,&byte,sizeof(byte),0);
	printf("rc = %d\n",rc);
	while (byte != 0x4242){
		rc = recv(sd,&byte,sizeof(byte),0);
		printf("recieved byte %c\n",byte);
		printf("rc = %d\n",rc);
		printf("compare to %c\n",0x4242);
	}
	int i = 0;
	while (byte != start_byte){
		rc = recv(sd,&byte,sizeof(byte),0);
		buffer[i] = byte;
		i = i+1;
	}
	printf("got %d bytes between start characters, %c\n",i,0x4242);
	memcpy(this->data,buffer,i*sizeof(this->data[0]));
}

void simple_tcp_client::send_data(char* msg){
	this->data_ready_sig = false;
	// write string to the server
	rc = write(sd, msg, strlen(msg));

	if(rc < 0) {
		//error.emit(ERR_ERR,"couldn't write to client");
		rc = getsockopt(sd, SOL_SOCKET, SO_ERROR, &temp, (socklen_t*)&length);
		if(rc == 0) {
			/* Print out the asynchronously received error. */
			errno = temp;
			//error.emit(ERR_ERR,"Socket_option was");
		}
		close(sd);
		exit(-1);
	} else {
		sprintf(this->err_str,"Client-write() is OK\n");
		sprintf(this->err_str,"String successfully sent!\n");
		//error.emit(ERR_DEBUG,err_str);
	}

	memset(buffer,0,sizeof(buffer));
	if (rc = recv(sd, &buffer,sizeof(buffer),0) <= 0){
		if(rc < 0) {
			//error.emit(ERR_ERR,"client read error");
			close(sd);
			exit(-1);
		} else if (rc == 0) {
			//error.emit(ERR_ERR,"Server program closed.");
			close(sd);
			exit(-1);
		}
	}
	sprintf(this->data,buffer);
	printf("number of bytes found is %d.\n",rc);
	printf("these bytes are: %s.\n",this->data);
	//error.emit(ERR_DEBUG,"Client-read() is OK");

	//data_ready.emit(buffer);
	this->data_ready_sig = true;
}

#endif
