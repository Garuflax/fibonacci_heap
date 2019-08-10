#include "utils.h"

unsigned int log2ul(unsigned long long int n){
    unsigned int res = 0;
    unsigned long long int temp = 1;
    while(temp < n){
        ++res;
        temp = temp << 1U;
    }
    return res;
}