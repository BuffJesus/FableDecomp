#include "rebuild_abi.h"
// _Dest_val<...> @ 0x006568bb
// push esi; mov esi,ecx; lea ecx,[esi+0x20]; call member_dtor; lea ecx,[esi+0x10]; pop esi; jmp base_dtor
// Destroy the embedded member at +0x20, then tail-jump the base destructor at +0x10.
struct FableMember_006568bb; struct FableBase_006568bb;
extern "C" void FABLE_FASTCALL FableMemberDtor_006568bb(FableMember_006568bb*);
extern "C" void FABLE_FASTCALL FableBaseDtor_006568bb(FableBase_006568bb*);
extern "C" void FABLE_FASTCALL FableDualDestval_006568bb(char *self)
{
    FableMemberDtor_006568bb((FableMember_006568bb*)(self + 0x20));
    FableBaseDtor_006568bb((FableBase_006568bb*)(self + 0x10));
}