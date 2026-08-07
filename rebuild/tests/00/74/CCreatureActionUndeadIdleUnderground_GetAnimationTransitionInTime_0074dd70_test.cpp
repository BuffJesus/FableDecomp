#include "rebuild_abi.h"
#include <cstdio>
struct C { char d; };
extern "C" int FABLE_FASTCALL FableRet_74dd70(C *self){ return 0; }
int main(){ C o; if(FableRet_74dd70(&o)!=0){std::printf("B1\n");return 1;}
 std::printf("RI_74dd70_OK\n"); return 0; }