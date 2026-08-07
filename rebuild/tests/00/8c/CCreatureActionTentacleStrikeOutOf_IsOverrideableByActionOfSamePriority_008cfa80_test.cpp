#include "rebuild_abi.h"
#include <cstdio>
struct C { char d; };
extern "C" char FABLE_FASTCALL FableRet_8cfa80(C *self){ return 1; }
int main(){ C o; if(FableRet_8cfa80(&o)!=1){std::printf("B1\n");return 1;}
 std::printf("RB_8cfa80_OK\n"); return 0; }