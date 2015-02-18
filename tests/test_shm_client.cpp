#include "../ah_shm.h"

int main(void){
	int a;
	shm_client<int> *some_var = new shm_client<int>(5678);
	a=some_var->get();
	printf("%d\n",a);

	return 0;
}