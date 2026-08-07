#include "rebuild_abi.h"

// _Dest_val<std::allocator<...>,...> @ 0x008fe477
// Destructor forwarder: destroys the embedded member at +0x14, then
// tail-jumps to the base/next destructor with this in ecx. Shape:
//   push esi; mov esi,ecx; lea ecx,[esi+0x14]; call member_dtor;
//   mov ecx,esi; pop esi; jmp base_dtor.
struct FableDestMember_008fe477;
struct FableDestHost_008fe477;

extern "C" void FABLE_FASTCALL FableMemberDtor_008fe477(FableDestMember_008fe477* m);
extern "C" void FABLE_FASTCALL FableBaseDtor_008fe477(FableDestHost_008fe477* self);

extern "C" void FABLE_FASTCALL FableDestVal_008fe477(FableDestHost_008fe477* self)
{
    FableMemberDtor_008fe477((FableDestMember_008fe477*)((char*)self + 0x14));
    FableBaseDtor_008fe477(self);
}