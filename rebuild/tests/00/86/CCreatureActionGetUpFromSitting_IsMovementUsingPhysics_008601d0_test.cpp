#include "rebuild_abi.h"
#include <cstdio>
struct C { char d; };
extern "C" char FABLE_FASTCALL FableRet_8601d0(C *self){ return 0; }
int main(){ C o; if(FableRet_8601d0(&o)!=0){std::printf("B1\n");return 1;}
 std::printf("RB_8601d0_OK\n"); return 0; }