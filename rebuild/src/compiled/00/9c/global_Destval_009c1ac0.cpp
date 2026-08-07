#include "rebuild_abi.h"
// _Dest_val<...> @ 0x009c1ac0
// push esi; mov esi,ecx; lea ecx,[esi+0x18]; call member_dtor; lea ecx,[esi+0x14]; pop esi; jmp base_dtor
// Destroy the embedded member at +0x18, then tail-jump the base destructor at +0x14.
struct FableMember_009c1ac0; struct FableBase_009c1ac0;
extern "C" void FABLE_FASTCALL FableMemberDtor_009c1ac0(FableMember_009c1ac0*);
extern "C" void FABLE_FASTCALL FableBaseDtor_009c1ac0(FableBase_009c1ac0*);
extern "C" void FABLE_FASTCALL FableDualDestval_009c1ac0(char *self)
{
    FableMemberDtor_009c1ac0((FableMember_009c1ac0*)(self + 0x18));
    FableBaseDtor_009c1ac0((FableBase_009c1ac0*)(self + 0x14));
}