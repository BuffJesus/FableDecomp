#include "rebuild_abi.h"
extern "C" long FABLE_STDCALL FableRet_ca3b10(double d){
    long r;
    __asm { fld d }
    __asm { fistp r }
    __asm { mov eax, r }
}