#include "rebuild_abi.h"

// _Dest_val<std::allocator<...>,...> @ 0x008fe094
// Destructor forwarder: destroys the embedded member at +0x1b, then
// tail-jumps to the base/next destructor with this in ecx. Shape:
//   push esi; mov esi,ecx; lea ecx,[esi+0x1b]; call member_dtor;
//   mov ecx,esi; pop esi; jmp base_dtor.
struct FableDestMember_008fe094;
struct FableDestHost_008fe094;

extern "C" void FABLE_FASTCALL FableMemberDtor_008fe094(FableDestMember_008fe094* m);
extern "C" void FABLE_FASTCALL FableBaseDtor_008fe094(FableDestHost_008fe094* self);

extern "C" void FABLE_FASTCALL FableDestVal_008fe094(FableDestHost_008fe094* self)
{
    FableMemberDtor_008fe094((FableDestMember_008fe094*)((char*)self + 0x1b));
    FableBaseDtor_008fe094(self);
}