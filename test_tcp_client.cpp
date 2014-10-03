#include "ah_tcp_client.h"
#include "sigslot.h"
#include <string>
#include <iostream>

class error : public sigslot::has_slots<>{
public:
	error(void);
	void printerror(int,char*);
	void printdata(char*);
};

error::error(void) {
	printf("error initialized.\n");
}

void error::printerror(int code,char* str){
	printf("ERROR CODE %d: %s\n",code,str);
}

void error::printdata(char* str){
	printf("[DATA]: %s\n",str);
}

int main(void){
	error *err_ind = new error();
	ah_tcp_client *client = new ah_tcp_client();
	client->error.connect(err_ind,&error::printerror);
	client->data_ready.connect(err_ind,&error::printdata);
	client->tcp_connect("192.168.1.40",10000);
	client->send_data("1");
	client->send_data("none");
	client->disconnect();

	return 0;
}