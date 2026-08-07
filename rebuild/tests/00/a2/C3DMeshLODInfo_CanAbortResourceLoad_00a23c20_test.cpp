#include "rebuild_abi.h"
#include <cstdio>
extern "C" int FABLE_STDCALL FableRet_a23c20(int a){ return a>1; }
int main(){ if(FableRet_a23c20(2)!=1){std::printf("B1");return 1;}
 if(FableRet_a23c20(1)!=0){std::printf("B2");return 1;}
 std::printf("RB_a23c20_OK\n"); return 0; }