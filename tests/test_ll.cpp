#include <stdio.h>
#include "ah_ll.h"

int main() {
        ll<double>* a = new ll<double>();
        a->insert(1.6);
        a->insert(2.6);
        a->insert(3.6);
        a->insert(4.6);
        a->insert(6.5);
        double val;
        printf("mean = %f\n",a->mean());
        printf("std = %f\n",a->stdev());
        while(!a->isempty()){
            printf("%f\n",a->pop());
        }
        a->destroy();
        printf("%f\n",a->pop());
        printf("%f\n",a->pop());
}
