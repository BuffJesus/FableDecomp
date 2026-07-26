#include "rebuild_abi.h"

/* sub_00404860: sets object's vtable pointer and returns this.
   mov eax,ecx ; mov [eax],0x122dc6c ; ret
   The stored value is a relocation-masked absolute address (a vtable). */

extern void* g_vtable_00404860; /* stands in for the 0x122dc6c global; addr is reloc-masked */

struct Obj_00404860 {
    void* vtbl; /* +0x00 */
};

Obj_00404860* __fastcall sub_00404860(Obj_00404860* thisptr)
{
    *(void**)thisptr = &g_vtable_00404860;
    return thisptr;
}