#include "rebuild_abi.h"
#include <cstdio>
struct C { char d; };
extern "C" char FABLE_FASTCALL FableRet_66a6d0(C *self){ return 0; }
int main(){ C o; if(FableRet_66a6d0(&o)!=0){std::printf("B1\n");return 1;}
 std::printf("RB_66a6d0_OK\n"); return 0; }