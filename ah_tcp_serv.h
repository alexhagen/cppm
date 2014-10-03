#pragma GCC diagnostic ignored "-Wwrite-strings"
#ifndef ah_tcp_serv_h
#define ah_tcp_serv_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include "sigslot.h"

#define DEFAULTPORT 9000

#define ERR_EMERG	0
#define ERR_CRIT	1
#define ERR_ERR		2
#define ERR_WARN	3
#define ERR_NOTICE	4
#define ERR_INFO	5
#define ERR_DEBUG	6



/*                AH TCP Multiple Client Server (Select) class                */
/*                             Author: Alex Hagen                             */
/* This server is a c++ implementation of the c select server.  I have strived 
	to make it signal and slot oriented, so that responses can be hooked up
	using the slots.  I have also strived to follow c++ practices and to make it
	robust using overloading and encapsulation, along with error handling and
	debugging.  */

class ah_tcp_serv : public sigslot::has_slots<>{
public:
	// constructor methods - overloading
    ah_tcp_serv(void);
    void change_ipaddr(char*);
    void change_port(int);
    // server start and stop methods for timing
    //void start_nb(void);
    void start_b(void);
    //void stop_nb(void);
    void send_msg(char*);
    void virtual send_reply(char*);
    // signals to hook up responses
	sigslot::signal1<char*> command_recv;
	sigslot::signal2<int,char*> error;

private:
	fd_set master; // master file descriptor list
    fd_set read_fds; // temp file descriptor list for select()
    struct sockaddr_in serveraddr; //server address
    struct sockaddr_in clientaddr; //client address
    int fdmax; // maximum file descriptor number
    int listener; // listening socket descriptor
    int newfd; // newly accept()ed socket descriptor
    char buf[1024]; // buffer for client data
    int nbytes;
    // for setsockopt() SO_REUSEADDR, below
    int yes = 1;
    int addrlen;
    int i, j;
    unsigned long int ipAddr = INADDR_ANY; // defaulting to 0.0.0.0
    int port = 9000; // defaulting to port 9000
    bool cont = true; // flag to continue in the loop
};

ah_tcp_serv::ah_tcp_serv(void) {
	// we don't need to do anything here
}


void ah_tcp_serv::change_ipaddr(char* _ipAddr) {
	// convert a char* ipAddr into the hex value
	ipAddr = inet_addr(_ipAddr);
}

void ah_tcp_serv::change_port(int _port) {
	// change the port to match that defined
	port = _port;
}

void ah_tcp_serv::start_b(void){
	/* get the listener */
    if((listener = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    	printf("listener error.\n");
        error.emit(ERR_CRIT, "Server-socket() cannot start listener.");
        exit(1);
    }
    //error.emit(ERR_DEBUG,"Server-socket() connected listener.");
    /*"address already in use" error message */
    if(setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == \
    	-1) {
    	printf("setsockopt error\n");
        error.emit(ERR_CRIT,"Server-setsockopt() error: ");
        exit(1);
    }
    error.emit(ERR_DEBUG,"Server-setsockopt() got the address and port.");
     
    // bind to the server address
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = ipAddr;
    serveraddr.sin_port = htons(port);
    memset(&(serveraddr.sin_zero), '\0', 8);
     
    if(bind(listener, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == \
    	-1) {
    	perror("bind error.\n");
        error.emit(ERR_CRIT,"Server-bind() failed to bind to server.");
        exit(1);
    }
    error.emit(ERR_DEBUG,"Server-bind() was able to bind to address.");
     
    // listen
    if(listen(listener, 10) == -1) {
    	printf("listen error.\n");
         error.emit(ERR_CRIT,"Server-listen() was unable to start listening.");
         exit(1);
    }
   	error.emit(ERR_DEBUG,"Server-listen() started listening.");

    FD_SET(listener, &master); // add the listener to the master set
    fdmax = listener; // keep track of the biggest file descriptor: 
    				  // so far, it's this one

    error.emit(ERR_INFO,"The tcp multiple client server has been initialized.");

	while(cont) {
        memset(buf,0,sizeof(buf));
        read_fds = master;
         
        if(select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1) {
            error.emit(ERR_ERR,"Server-select had an error!");
            //perror("Server-select() error lol!");
            exit(1);
        }
        error.emit(ERR_DEBUG,"server was able to select.");
         
        /*run through the existing connections looking for data to be read*/
        for(i = 0; i <= fdmax; i++) {
            if(FD_ISSET(i, &read_fds)) { /* we got one... */
	            if(i == listener) {
	                 /* handle new connections */
	                addrlen = sizeof(clientaddr);
	                if((newfd = accept(listener, (struct sockaddr *)&clientaddr, (socklen_t*)&addrlen)) == -1) {
	                    error.emit(ERR_ERR,"Server-accept() error.");
	                } else {
	                    error.emit(ERR_DEBUG,"Server accept is okay.");
	                    FD_SET(newfd, &master); /* add to master set */
	                    if(newfd > fdmax) { /* keep track of the maximum */
	                        fdmax = newfd;
	                    }
	                    char err_buff[255];
	                    sprintf(err_buff,"%s: New connection from %s on socket %d", "program name", inet_ntoa(clientaddr.sin_addr), newfd);
	                    error.emit(ERR_INFO,err_buff);
	                }
	            } else {
                    /* handle data from a client */
                    if((nbytes = recv(i, buf, sizeof(buf), 0)) <= 0) {
                        // got error or connection closed by client
                        if(nbytes == 0) {
	                        // connection closed
	                        char err_buff[255];
	                        sprintf(err_buff,"%s: socket %d hung up", "program name", i);
	                        error.emit(ERR_INFO,err_buff);
                        } else {
                            perror("recieve error()");
                            error.emit(ERR_WARN,"receive error().");
                        }
                        // close it...
                        close(i);
                        // remove from master set
                        FD_CLR(i, &master);
                    } else {
                        printf("string received was \"%s\"\n",buf);
                    	send_reply(buf);
                    }
	            }
            }
        }
    }
}

void ah_tcp_serv::send_reply(char* buf){
    char *rply="\0";
	if(send(i,rply,strlen(rply),0)==-1) {
        error.emit(ERR_ERR,"Send didn't complete.");
    }
}

void ah_tcp_serv::send_msg(char* rply){
	// reply back to the sender
    if(send(i,rply,strlen(rply),0)==-1) {
    	error.emit(ERR_ERR,"Send didn't complete.");
    }
}

#endif