#include "rebuild_abi.h"

// Retail 0x0041cd01: writes a vtable/constant pointer into this+0, returns this.
// mov eax,ecx ; mov dword ptr [eax],0x122ffcc ; ret

// The stored constant is an absolute address (relocation-masked in parity).
extern void* const vtbl_0041cd01[];

struct Obj_0041cd01 {
    void* vtbl;
};

Obj_0041cd01* __fastcall sub_0041cd01(Obj_0041cd01* thisptr)
{
    thisptr->vtbl = (void*)vtbl_0041cd01;
    return thisptr;
}