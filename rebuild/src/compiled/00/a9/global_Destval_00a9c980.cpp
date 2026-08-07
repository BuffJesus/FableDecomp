#include "rebuild_abi.h"

// _Dest_val<std::allocator<...>,...> @ 0x00a9c980
// Destructor forwarder: destroys the embedded member at +0x40, then
// tail-jumps to the base/next destructor with this in ecx. Shape:
//   push esi; mov esi,ecx; lea ecx,[esi+0x40]; call member_dtor;
//   mov ecx,esi; pop esi; jmp base_dtor.
struct FableDestMember_00a9c980;
struct FableDestHost_00a9c980;

extern "C" void FABLE_FASTCALL FableMemberDtor_00a9c980(FableDestMember_00a9c980* m);
extern "C" void FABLE_FASTCALL FableBaseDtor_00a9c980(FableDestHost_00a9c980* self);

extern "C" void FABLE_FASTCALL FableDestVal_00a9c980(FableDestHost_00a9c980* self)
{
    FableMemberDtor_00a9c980((FableDestMember_00a9c980*)((char*)self + 0x40));
    FableBaseDtor_00a9c980(self);
}