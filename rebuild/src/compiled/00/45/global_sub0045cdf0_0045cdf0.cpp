#pragma optimize("s",on)
#include "rebuild_abi.h"

// Retail 0x0045cdf0: scalar `new` wrapper.
// push 0x54 ; call operator new ; test eax,eax ; pop ecx ; je null
// mov ecx,eax ; jmp ctor (tail-call, this=eax) ; null: xor eax,eax ; ret

// engine operator new (relocation-masked target)
extern void* __cdecl engine_operator_new(unsigned int sz);

// __fastcall constructor of the 0x54-byte object, modeled as __fastcall.
extern void* __fastcall obj_ctor_45cdf0(void* thisptr);

void* __fastcall sub_0045cdf0()
{
    void* p = engine_operator_new(0x54);
    if (p)
        return obj_ctor_45cdf0(p);
    return 0;
}