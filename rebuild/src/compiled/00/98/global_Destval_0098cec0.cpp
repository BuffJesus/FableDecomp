#include "rebuild_abi.h"

// _Dest_val<std::allocator<...>,...> @ 0x0098cec0
// Destructor forwarder: destroys the embedded member at +0x4, then
// tail-jumps to the base/next destructor with this in ecx. Shape:
//   push esi; mov esi,ecx; lea ecx,[esi+0x4]; call member_dtor;
//   mov ecx,esi; pop esi; jmp base_dtor.
struct FableDestMember_0098cec0;
struct FableDestHost_0098cec0;

extern "C" void FABLE_FASTCALL FableMemberDtor_0098cec0(FableDestMember_0098cec0* m);
extern "C" void FABLE_FASTCALL FableBaseDtor_0098cec0(FableDestHost_0098cec0* self);

extern "C" void FABLE_FASTCALL FableDestVal_0098cec0(FableDestHost_0098cec0* self)
{
    FableMemberDtor_0098cec0((FableDestMember_0098cec0*)((char*)self + 0x4));
    FableBaseDtor_0098cec0(self);
}