#include "rebuild_abi.h"

// _Dest_val<std::allocator<...>,...> @ 0x008fe006
// Destructor forwarder: destroys the embedded member at +0x14, then tail-jumps
// to the base/next destructor with this in ecx. Shape:
//   push esi; mov esi,ecx; lea ecx,[esi+0x14]; call member_dtor;
//   mov ecx,esi; pop esi; jmp base_dtor.
struct FableDestMember_008fe006;
struct FableDestHost_008fe006
{
    char pad[0x14];
    FableDestMember_008fe006* member_at_14;
};

extern "C" void FABLE_FASTCALL FableMemberDtor_008fe006(FableDestMember_008fe006* m);
extern "C" void FABLE_FASTCALL FableBaseDtor_008fe006(FableDestHost_008fe006* self);

extern "C" void FABLE_FASTCALL FableDestVal_008fe006(FableDestHost_008fe006* self)
{
    FableMemberDtor_008fe006((FableDestMember_008fe006*)&self->member_at_14);
    FableBaseDtor_008fe006(self);
}