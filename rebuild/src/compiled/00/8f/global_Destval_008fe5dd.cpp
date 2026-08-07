#include "rebuild_abi.h"

// _Dest_val<std::allocator<...>,...> @ 0x008fe5dd
// Destructor forwarder: destroys the embedded member at +0x14, then
// tail-jumps to the base/next destructor with this in ecx. Shape:
//   push esi; mov esi,ecx; lea ecx,[esi+0x14]; call member_dtor;
//   mov ecx,esi; pop esi; jmp base_dtor.
struct FableDestMember_008fe5dd;
struct FableDestHost_008fe5dd;

extern "C" void FABLE_FASTCALL FableMemberDtor_008fe5dd(FableDestMember_008fe5dd* m);
extern "C" void FABLE_FASTCALL FableBaseDtor_008fe5dd(FableDestHost_008fe5dd* self);

extern "C" void FABLE_FASTCALL FableDestVal_008fe5dd(FableDestHost_008fe5dd* self)
{
    FableMemberDtor_008fe5dd((FableDestMember_008fe5dd*)((char*)self + 0x14));
    FableBaseDtor_008fe5dd(self);
}