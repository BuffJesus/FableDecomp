#include "rebuild_abi.h"

// _Dest_val<std::allocator<...>,...> @ 0x008fd8d7
// Destructor forwarder: destroys the embedded member at +0x14, then tail-jumps
// to the base/next destructor with this in ecx. Shape:
//   push esi; mov esi,ecx; lea ecx,[esi+0x14]; call member_dtor;
//   mov ecx,esi; pop esi; jmp base_dtor.
struct FableDestMember_008fd8d7;
struct FableDestHost_008fd8d7
{
    char pad[0x14];
    FableDestMember_008fd8d7* member_at_14;
};

extern "C" void FABLE_FASTCALL FableMemberDtor_008fd8d7(FableDestMember_008fd8d7* m);
extern "C" void FABLE_FASTCALL FableBaseDtor_008fd8d7(FableDestHost_008fd8d7* self);

extern "C" void FABLE_FASTCALL FableDestVal_008fd8d7(FableDestHost_008fd8d7* self)
{
    FableMemberDtor_008fd8d7((FableDestMember_008fd8d7*)&self->member_at_14);
    FableBaseDtor_008fd8d7(self);
}