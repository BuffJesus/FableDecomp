#include "rebuild_abi.h"

// _Dest_val<std::allocator<...>,...> @ 0x00a3fd40
// Destructor forwarder: destroys the embedded member at +0x4, then
// tail-jumps to the base/next destructor with this in ecx. Shape:
//   push esi; mov esi,ecx; lea ecx,[esi+0x4]; call member_dtor;
//   mov ecx,esi; pop esi; jmp base_dtor.
struct FableDestMember_00a3fd40;
struct FableDestHost_00a3fd40;

extern "C" void FABLE_FASTCALL FableMemberDtor_00a3fd40(FableDestMember_00a3fd40* m);
extern "C" void FABLE_FASTCALL FableBaseDtor_00a3fd40(FableDestHost_00a3fd40* self);

extern "C" void FABLE_FASTCALL FableDestVal_00a3fd40(FableDestHost_00a3fd40* self)
{
    FableMemberDtor_00a3fd40((FableDestMember_00a3fd40*)((char*)self + 0x4));
    FableBaseDtor_00a3fd40(self);
}