#include "rebuild_abi.h"
#include <cstdio>
struct C { char d; };
extern "C" char FABLE_FASTCALL FableRet_8cf970(C *self){ return 0; }
int main(){ C o; if(FableRet_8cf970(&o)!=0){std::printf("B1\n");return 1;}
 std::printf("RB_8cf970_OK\n"); return 0; }