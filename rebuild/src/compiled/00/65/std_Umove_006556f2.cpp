#pragma optimize("s",on)
#include "rebuild_abi.h"

// vector<EWeaponClass>::_Umove @ 0x006556f2
// push ebp; mov ebp,esp; lea eax,[ebp+0xb]; push eax; push [ebp+8];
// call construct; pop ebp; ret 4   (stdcall, one pointer arg + stack local)
struct FableUmoveElem_006556f2;
void FABLE_STDCALL FableUmove_construct_006556f2(FableUmoveElem_006556f2* p, char* local);

extern "C" void FABLE_STDCALL FableUmove_006556f2(FableUmoveElem_006556f2* p)
{
    char local;
    FableUmove_construct_006556f2(p, &local);
}