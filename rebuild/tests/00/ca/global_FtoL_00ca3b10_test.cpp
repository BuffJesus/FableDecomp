#include "rebuild_abi.h"
#include <cstdio>
extern "C" long FABLE_STDCALL FableRet_ca3b10(double d){
    long r;
    __asm { fld d }
    __asm { fistp r }
    __asm { mov eax, r }
}
int main(){
 if(FableRet_ca3b10(3.9)!=4){std::printf("B1");return 1;}
 if(FableRet_ca3b10(-2.5)!=-2){std::printf("B2");return 1;}
 std::printf("RB_ca3b10_OK\n"); return 0; }