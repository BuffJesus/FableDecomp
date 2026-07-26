#pragma optimize("s",on)
#include "rebuild_abi.h"

struct CLandscapeBackgroundPatch {
    void *vtbl;
};

// scalar destructor (the masked call target)
extern "C" void __fastcall CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch *thisptr);
// operator delete
extern "C" void __cdecl sub_operator_delete(void *p);

// vector deleting destructor, retail 0x00459aa2
// __fastcall(this, unsigned int flags) modeled as __fastcall(this, edx, flags)
void * __fastcall CLandscapeBackgroundPatch_vector_deleting_dtor(
        CLandscapeBackgroundPatch *thisptr, int /*edx*/, unsigned int flags)
{
    CLandscapeBackgroundPatch *self = thisptr;
    CLandscapeBackgroundPatch_dtor(self);
    if (flags & 1) {
        sub_operator_delete(self);
    }
    return self;
}