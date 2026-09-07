#include <cstdio>
static long Get(bool hero,bool invalid,bool capable,long first,long second){return hero&&!invalid&&capable?first+second:0;}
int main(){if(Get(true,false,true,3,4)!=7||Get(false,false,true,3,4)||Get(true,true,true,3,4)||Get(true,false,false,3,4))return 1;puts("GET_NUM_HOUSES_OWNED_PASS");return 0;}