#include "rebuild_abi.h"
// _Dest_val<...> @ 0x005edcf0
// call H1; mov ecx,eax; call H2; mov ecx,[eax+0xc]; jmp H3
// Resolve two singletons, then tail-jump a destructor on the +0xc member of the second.
struct A; struct B;
extern "C" A * FABLE_CDECL FableGetA_5edcf0();
extern "C" B * FABLE_FASTCALL FableGetB_5edcf0(A *a);
extern "C" void FABLE_FASTCALL FableDtorC_5edcf0(void *c);
extern "C" void FABLE_CDECL FableDestvalChain_5edcf0()
{
    B *b = FableGetB_5edcf0(FableGetA_5edcf0());
    FableDtorC_5edcf0(*(void**)((char*)b + 0xc));
}