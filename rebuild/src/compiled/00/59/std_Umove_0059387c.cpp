#pragma optimize("s",on)
#include "rebuild_abi.h"

// vector<EWeaponClass>::_Umove @ 0x0059387c
// push ebp; mov ebp,esp; lea eax,[ebp+0xb]; push eax; push [ebp+8];
// call construct; pop ebp; ret 4   (stdcall, one pointer arg + stack local)
struct FableUmoveElem_0059387c;
void FABLE_STDCALL FableUmove_construct_0059387c(FableUmoveElem_0059387c* p, char* local);

extern "C" void FABLE_STDCALL FableUmove_0059387c(FableUmoveElem_0059387c* p)
{
    char local;
    FableUmove_construct_0059387c(p, &local);
}