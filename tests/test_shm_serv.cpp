#include "../ah_shm.h"

int main(void){

	shm_server<int> *some_var = new shm_server<int>(5678);
	some_var->set(5);

	while (*some_var->shm != '*')
        sleep(1);

	return 0;
}