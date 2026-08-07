#include "rebuild_abi.h"
// _Dest_val<...> @ 0x00550a00
// push esi; mov esi,ecx; lea ecx,[esi+0x58]; call member_dtor; lea ecx,[esi+0x24]; pop esi; jmp base_dtor
// Destroy the embedded member at +0x58, then tail-jump the base destructor at +0x24.
struct FableMember_00550a00; struct FableBase_00550a00;
extern "C" void FABLE_FASTCALL FableMemberDtor_00550a00(FableMember_00550a00*);
extern "C" void FABLE_FASTCALL FableBaseDtor_00550a00(FableBase_00550a00*);
extern "C" void FABLE_FASTCALL FableDualDestval_00550a00(char *self)
{
    FableMemberDtor_00550a00((FableMember_00550a00*)(self + 0x58));
    FableBaseDtor_00550a00((FableBase_00550a00*)(self + 0x24));
}