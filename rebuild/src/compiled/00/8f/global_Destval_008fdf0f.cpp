#include "rebuild_abi.h"

// _Dest_val<std::allocator<...>,...> @ 0x008fdf0f
// Destructor forwarder: destroys the embedded member at +0x14, then tail-jumps
// to the base/next destructor with this in ecx. Shape:
//   push esi; mov esi,ecx; lea ecx,[esi+0x14]; call member_dtor;
//   mov ecx,esi; pop esi; jmp base_dtor.
struct FableDestMember_008fdf0f;
struct FableDestHost_008fdf0f
{
    char pad[0x14];
    FableDestMember_008fdf0f* member_at_14;
};

extern "C" void FABLE_FASTCALL FableMemberDtor_008fdf0f(FableDestMember_008fdf0f* m);
extern "C" void FABLE_FASTCALL FableBaseDtor_008fdf0f(FableDestHost_008fdf0f* self);

extern "C" void FABLE_FASTCALL FableDestVal_008fdf0f(FableDestHost_008fdf0f* self)
{
    FableMemberDtor_008fdf0f((FableDestMember_008fdf0f*)&self->member_at_14);
    FableBaseDtor_008fdf0f(self);
}