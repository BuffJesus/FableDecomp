#include "rebuild_abi.h"

// _Dest_val<std::allocator<...>,...> @ 0x008fddaa
// Destructor forwarder: destroys the embedded member at +0x14, then tail-jumps
// to the base/next destructor with this in ecx. Shape:
//   push esi; mov esi,ecx; lea ecx,[esi+0x14]; call member_dtor;
//   mov ecx,esi; pop esi; jmp base_dtor.
struct FableDestMember_008fddaa;
struct FableDestHost_008fddaa
{
    char pad[0x14];
    FableDestMember_008fddaa* member_at_14;
};

extern "C" void FABLE_FASTCALL FableMemberDtor_008fddaa(FableDestMember_008fddaa* m);
extern "C" void FABLE_FASTCALL FableBaseDtor_008fddaa(FableDestHost_008fddaa* self);

extern "C" void FABLE_FASTCALL FableDestVal_008fddaa(FableDestHost_008fddaa* self)
{
    FableMemberDtor_008fddaa((FableDestMember_008fddaa*)&self->member_at_14);
    FableBaseDtor_008fddaa(self);
}