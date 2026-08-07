#include "rebuild_abi.h"
#include <cstdio>
struct C { char d; };
extern "C" int FABLE_FASTCALL FableRet_8659e0(C *self){ return 0; }
int main(){ C o; if(FableRet_8659e0(&o)!=0){std::printf("B1\n");return 1;}
 std::printf("RI_8659e0_OK\n"); return 0; }