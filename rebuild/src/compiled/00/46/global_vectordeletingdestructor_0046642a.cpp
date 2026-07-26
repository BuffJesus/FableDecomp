#pragma optimize("s",on)
#include "rebuild_abi.h"

// vector deleting destructor @ 0x0046642a (CLandscapeBackgroundPatch)
// this in ecx, flags on stack; sets vftable, calls dtor, conditionally frees, returns this; ret 4

struct CLandscapeBackgroundPatch {
    void** vftable;
};

extern void* g_CLandscapeBackgroundPatch_vftable; // vtable @ 0x1230ba0
extern void __fastcall CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch* thisptr); // 0x596126
extern void __cdecl engine_operator_delete(void* p); // 0x798592

void* __fastcall CLandscapeBackgroundPatch_vector_dtor(
    CLandscapeBackgroundPatch* thisptr, int /*edx*/, unsigned int flags)
{
    thisptr->vftable = (void**)&g_CLandscapeBackgroundPatch_vftable;
    CLandscapeBackgroundPatch_dtor(thisptr);
    if (flags & 1) {
        engine_operator_delete(thisptr);
    }
    return thisptr;
}