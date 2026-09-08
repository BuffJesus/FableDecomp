#include <stdio.h>
int main(){unsigned __int64 n=0xffffffffULL;unsigned __int64 old=n++;if(old!=0xffffffffULL||n!=0x100000000ULL)return 1;printf("MAP_THING_UID PASS\n");return 0;}