#pragma optimize("s",on)
#include "rebuild_abi.h"

// vector<EWeaponClass>::_Umove @ 0x005f9a29
// push ebp; mov ebp,esp; lea eax,[ebp+0xb]; push eax; push [ebp+8];
// call construct; pop ebp; ret 4   (stdcall, one pointer arg + stack local)
struct FableUmoveElem_005f9a29;
void FABLE_STDCALL FableUmove_construct_005f9a29(FableUmoveElem_005f9a29* p, char* local);

extern "C" void FABLE_STDCALL FableUmove_005f9a29(FableUmoveElem_005f9a29* p)
{
    char local;
    FableUmove_construct_005f9a29(p, &local);
}