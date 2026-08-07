#include <cstdio>
struct Y { char pad[0x26]; unsigned char flag; };
struct X { char pad[8]; Y* y; };
static unsigned char IsHitBlockedCall(X* arg){ return arg->y->flag; }
int main(){ Y yy; yy.flag=0x7C; X xx; xx.y=&yy;
 if(IsHitBlockedCall(&xx)!=0x7C){ std::printf("00810f50_TEST FAIL\n"); return 1;}
 std::printf("00810f50_TEST PASS\n"); return 0;}