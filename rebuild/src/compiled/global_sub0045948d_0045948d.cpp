#pragma optimize("s",on)
#include "rebuild_abi.h"

// Retail 0x0045948d: operator new(0x74) + construct + set vtable pointer, return object ptr.
// push 0x74 ; call operator new(0x7a558d) ; if null -> return 0
// else: mov ecx,esi ; call ctor(0x21de43) ; mov [esi],0x12332cc (vtable) ; return esi
struct Obj_0045948d;

extern "C" void* __cdecl op_new_0045948d(unsigned int size);
extern void __fastcall ctor_0045948d(Obj_0045948d* thisptr);
extern const void* vtbl_0045948d; // absolute immediate operand, relocation-masked

struct Obj_0045948d
{
    const void* vptr;
};

Obj_0045948d* __fastcall sub_0045948d()
{
    Obj_0045948d* p = (Obj_0045948d*)op_new_0045948d(0x74);
    if (p)
    {
        ctor_0045948d(p);
        p->vptr = &vtbl_0045948d;
        return p;
    }
    return 0;
}