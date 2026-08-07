#include "rebuild_abi.h"
#include <cstdio>
struct C { char d; };
extern "C" char FABLE_FASTCALL FableRet_74db90(C *self){ return 1; }
int main(){ C o; if(FableRet_74db90(&o)!=1){std::printf("B1\n");return 1;}
 std::printf("RB_74db90_OK\n"); return 0; }