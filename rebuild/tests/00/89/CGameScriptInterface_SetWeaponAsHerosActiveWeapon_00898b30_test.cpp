#include <cstdio>
static bool Activate(long index,bool hero,unsigned char flags,unsigned long capabilities,long count){return index>0&&hero&&!(flags&1)&&(capabilities&0x80000)&&count>0;}
int main(){if(!Activate(4,true,0,0x80000,1)||Activate(0,true,0,0x80000,1)||Activate(4,true,1,0x80000,1)||Activate(4,true,0,0x80000,0))return 1;puts("ACTIVE_WEAPON_PASS");return 0;}