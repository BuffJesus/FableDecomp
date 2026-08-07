#include "rebuild_abi.h"

// _Dest_val<std::allocator<...>,...> @ 0x00cd44e0
// Destructor forwarder: destroys the embedded member at +0x4, then
// tail-jumps to the base/next destructor with this in ecx. Shape:
//   push esi; mov esi,ecx; lea ecx,[esi+0x4]; call member_dtor;
//   mov ecx,esi; pop esi; jmp base_dtor.
struct FableDestMember_00cd44e0;
struct FableDestHost_00cd44e0;

extern "C" void FABLE_FASTCALL FableMemberDtor_00cd44e0(FableDestMember_00cd44e0* m);
extern "C" void FABLE_FASTCALL FableBaseDtor_00cd44e0(FableDestHost_00cd44e0* self);

extern "C" void FABLE_FASTCALL FableDestVal_00cd44e0(FableDestHost_00cd44e0* self)
{
    FableMemberDtor_00cd44e0((FableDestMember_00cd44e0*)((char*)self + 0x4));
    FableBaseDtor_00cd44e0(self);
}