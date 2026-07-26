#pragma optimize("s",on)
#include "rebuild_abi.h"

struct CLandscapeBackgroundPatch;

// scalar destructor (this-only member), relocation-masked call target
extern void __fastcall CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch* thisptr);
// operator delete (cdecl), relocation-masked call target
extern void __cdecl fable_operator_delete(void* p);

// `vector_deleting_destructor' @ 0x0046ef6b
// __fastcall void* dtor(unsigned int flags) modeled as __fastcall(this, edx, flags)
void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(
    CLandscapeBackgroundPatch* thisptr, int /*edx*/, unsigned int flags)
{
    CLandscapeBackgroundPatch_dtor(thisptr);
    if (flags & 1)
        fable_operator_delete(thisptr);
    return thisptr;
}