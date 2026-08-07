#include "rebuild_abi.h"
#include <cstdio>
extern "C" int FABLE_STDCALL FableRet_bb6dc0(int a){ return a==0x19; }
int main(){ if(FableRet_bb6dc0(0x19)!=1){std::printf("B1");return 1;}
 if(FableRet_bb6dc0(3)!=0){std::printf("B2");return 1;}
 std::printf("RB_bb6dc0_OK\n"); return 0; }