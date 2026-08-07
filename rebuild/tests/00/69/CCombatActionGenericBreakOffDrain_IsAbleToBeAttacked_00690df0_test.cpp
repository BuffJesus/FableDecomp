#include "rebuild_abi.h"
#include <cstdio>
struct C { char d; };
extern "C" char FABLE_FASTCALL FableRet_690df0(C *self){ return 0; }
int main(){ C o; if(FableRet_690df0(&o)!=0){std::printf("B1\n");return 1;}
 std::printf("RB_690df0_OK\n"); return 0; }