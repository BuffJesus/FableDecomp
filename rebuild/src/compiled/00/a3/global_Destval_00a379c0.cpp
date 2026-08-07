#include "rebuild_abi.h"

// _Dest_val<std::allocator<...>,...> @ 0x00a379c0
// Destructor forwarder: destroys the embedded member at +0x10, then
// tail-jumps to the base/next destructor with this in ecx. Shape:
//   push esi; mov esi,ecx; lea ecx,[esi+0x10]; call member_dtor;
//   mov ecx,esi; pop esi; jmp base_dtor.
struct FableDestMember_00a379c0;
struct FableDestHost_00a379c0;

extern "C" void FABLE_FASTCALL FableMemberDtor_00a379c0(FableDestMember_00a379c0* m);
extern "C" void FABLE_FASTCALL FableBaseDtor_00a379c0(FableDestHost_00a379c0* self);

extern "C" void FABLE_FASTCALL FableDestVal_00a379c0(FableDestHost_00a379c0* self)
{
    FableMemberDtor_00a379c0((FableDestMember_00a379c0*)((char*)self + 0x10));
    FableBaseDtor_00a379c0(self);
}