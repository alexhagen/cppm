#include "simple_tcp_client.h"
#include <string>
#include <iostream>

class xbee : public simple_tcp_client {
public:
    int decode(void);
};

int xbee::decode(){
    this->accumulate(16);
    int val = 0;
		int val2 = 0;
    val = uint8_t(this->data[12]) * 256 + uint8_t(this->data[13]);
		val2 = uint8_t(this->data[14]) * 256 + uint8_t(this->data[15]);
		printf("%d %d\n",val,val2);
    return val;
    //printf("%4d %4d %4d %4d %4d %4d %4d %4d %4d %4d %4d %4d %4d %4d:\t\t%4d\n",
    //    data[0],data[1],data[2],data[3],data[4],data[5],data[6],data[7],data[8],
    //    data[9],data[10],data[11],data[12],data[13],val);
}

int main(void){
	// connect to the wifi xbee through tcp
  xbee *xbwf = new xbee();
  xbwf->tcp_connect("192.168.1.6",9750);


  while(1){
      xbwf->decode();
      usleep(50000);
  }
  xbwf->disconnect();
  return 0;

	return 0;
}
