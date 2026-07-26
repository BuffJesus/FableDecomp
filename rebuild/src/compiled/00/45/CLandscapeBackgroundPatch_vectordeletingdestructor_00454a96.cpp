#pragma optimize("s",on)
#include <new>
struct CLandscapeBackgroundPatch { int f0; };
extern void __fastcall CLandscapeBackgroundPatch_scalar_dtor(CLandscapeBackgroundPatch* self);

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edx_unused, unsigned int flags)
{
    CLandscapeBackgroundPatch_scalar_dtor(self);
    if (flags & 1)
        ::operator delete(self);
    return self;
}