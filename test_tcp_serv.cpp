#include "ah_tcp_serv.h"
#include "sigslot.h"

class error : public sigslot::has_slots<>{
public:
	error(void);
	void printerror(int,char*);
};

error::error(void) {
	printf("error initialized.\n");
}

void error::printerror(int code,char* str){
	printf("ERROR CODE %d: %s\n",code,str);
}

class reply : public ah_tcp_serv {
public:
	reply(void) : ah_tcp_serv(){};
	reply(char* _ipAddr) : ah_tcp_serv(_ipAddr) {};
	reply(int _port) : ah_tcp_serv(_port) {};
	reply(char* _ipAddr,int _port) : ah_tcp_serv(_ipAddr, _port) {};
	void send_reply(char*);
};

void reply::send_reply(char* buf) {
	char rply[1024];
	if (atoi(buf) == 1){
		sprintf(rply,"command 1 sent.");
	} else {
		sprintf(rply,"no recognizable command sent.");
	}
	send_msg(rply);
}

int main(void){
	error *err_ind = new error();
	reply *server = new reply(10000);
	server->error.connect(err_ind,&error::printerror);
	server->start_b();

	return 0;
}