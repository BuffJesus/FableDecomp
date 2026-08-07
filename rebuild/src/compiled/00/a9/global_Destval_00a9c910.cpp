#include "rebuild_abi.h"

// _Dest_val<std::allocator<...>,...> @ 0x00a9c910
// Destructor forwarder: destroys the embedded member at +0x6c, then
// tail-jumps to the base/next destructor with this in ecx. Shape:
//   push esi; mov esi,ecx; lea ecx,[esi+0x6c]; call member_dtor;
//   mov ecx,esi; pop esi; jmp base_dtor.
struct FableDestMember_00a9c910;
struct FableDestHost_00a9c910;

extern "C" void FABLE_FASTCALL FableMemberDtor_00a9c910(FableDestMember_00a9c910* m);
extern "C" void FABLE_FASTCALL FableBaseDtor_00a9c910(FableDestHost_00a9c910* self);

extern "C" void FABLE_FASTCALL FableDestVal_00a9c910(FableDestHost_00a9c910* self)
{
    FableMemberDtor_00a9c910((FableDestMember_00a9c910*)((char*)self + 0x6c));
    FableBaseDtor_00a9c910(self);
}