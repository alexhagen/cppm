#include <stdio.h>
#include "ah_cll.h"

int main() {
        cll<double>* a = new cll<double>(3);
        a->insert(1.6);
        a->insert(2.6);
        a->insert(3.6);
        a->insert(4.6);
        a->insert(6.5);
        while(a->size > 0){
          printf("size: %d\n",a->size);
          printf("%f\n",a->pop());
        }
}
