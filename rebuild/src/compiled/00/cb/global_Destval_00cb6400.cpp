#include "rebuild_abi.h"

// _Dest_val<std::allocator<...>,...> @ 0x00cb6400
// Destructor forwarder: destroys the embedded member at +0xc, then
// tail-jumps to the base/next destructor with this in ecx. Shape:
//   push esi; mov esi,ecx; lea ecx,[esi+0xc]; call member_dtor;
//   mov ecx,esi; pop esi; jmp base_dtor.
struct FableDestMember_00cb6400;
struct FableDestHost_00cb6400;

extern "C" void FABLE_FASTCALL FableMemberDtor_00cb6400(FableDestMember_00cb6400* m);
extern "C" void FABLE_FASTCALL FableBaseDtor_00cb6400(FableDestHost_00cb6400* self);

extern "C" void FABLE_FASTCALL FableDestVal_00cb6400(FableDestHost_00cb6400* self)
{
    FableMemberDtor_00cb6400((FableDestMember_00cb6400*)((char*)self + 0xc));
    FableBaseDtor_00cb6400(self);
}