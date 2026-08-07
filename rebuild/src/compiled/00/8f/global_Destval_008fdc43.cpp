#include "rebuild_abi.h"

// _Dest_val<std::allocator<...>,...> @ 0x008fdc43
// Destructor forwarder: destroys the embedded member at +0x14, then tail-jumps
// to the base/next destructor with this in ecx. Shape:
//   push esi; mov esi,ecx; lea ecx,[esi+0x14]; call member_dtor;
//   mov ecx,esi; pop esi; jmp base_dtor.
struct FableDestMember_008fdc43;
struct FableDestHost_008fdc43
{
    char pad[0x14];
    FableDestMember_008fdc43* member_at_14;
};

extern "C" void FABLE_FASTCALL FableMemberDtor_008fdc43(FableDestMember_008fdc43* m);
extern "C" void FABLE_FASTCALL FableBaseDtor_008fdc43(FableDestHost_008fdc43* self);

extern "C" void FABLE_FASTCALL FableDestVal_008fdc43(FableDestHost_008fdc43* self)
{
    FableMemberDtor_008fdc43((FableDestMember_008fdc43*)&self->member_at_14);
    FableBaseDtor_008fdc43(self);
}