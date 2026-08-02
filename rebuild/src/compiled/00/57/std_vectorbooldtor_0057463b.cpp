#pragma optimize("s",on)
#include "rebuild_abi.h"

// std::vector<bool, std::allocator<bool>>::~vector() -- retail 0x0057463b
//
// Layout (from retail bytes 838904020000ffe9efdfffff):
//   or  dword ptr [ecx+0x204], 0xffffffff   ; invalidate/poison a member slot
//   jmp BaseDtor                             ; tail-jmp to 0x00572636 (base
//                                             ; _Vector_val/_Bvector_reachable
//                                             ; style cleanup); no further code
//                                             ; after -- a real, expressible
//                                             ; tail call in C++, not asm.
//
// The "or reg/mem, -1" idiom always yields -1 regardless of prior contents,
// so it is semantically an unconditional assignment of 0xffffffff to that
// dword slot (commonly a sentinel/"already destroyed" pattern for the vector
// storage pointer/size pair in this local vector<bool> layout).

struct FableVectorBool
{
    unsigned char bytes[0x208];
};

// Base destructor this thunk tail-calls into -- a separate compiled unit at
// 0x00572636, treated as an opaque external symbol so it is invoked via a
// real (tail) call rather than inlined, matching the retail jmp.
extern "C" void FABLE_FASTCALL
FableVectorBool_BaseDtor_00572636(void* self);

extern "C" void FABLE_FASTCALL
FableVectorBool_dtor_0057463b(FableVectorBool* this_)
{
    *reinterpret_cast<unsigned long*>(this_->bytes + 0x204) = 0xfffffffful;
    FableVectorBool_BaseDtor_00572636(this_);
}