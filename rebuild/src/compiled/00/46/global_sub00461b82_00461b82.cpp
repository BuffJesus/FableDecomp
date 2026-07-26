#pragma optimize("s",on)
#include "rebuild_abi.h"

/* 0x00461b82 : allocator + placement-ctor wrapper.
 *   push 0x7c ; call operator_new(0x7c) ; if(!p) return 0 ;
 *   ecx=p ; tail-jmp ctor(p) ; return p
 * Modeled instruction-for-instruction. The ctor is __fastcall (this in ecx);
 * we express it as __fastcall(objptr) so the this-in-ecx + tail return match. */

struct SomeType;

extern "C" void* __cdecl engine_op_new_461b82(unsigned int size);
extern SomeType* __fastcall SomeType_ctor_461b82(SomeType* thisptr);

SomeType* __fastcall make_461b82(void)
{
    SomeType* p = (SomeType*)engine_op_new_461b82(0x7c);
    if (p != 0)
        return SomeType_ctor_461b82(p);
    return 0;
}