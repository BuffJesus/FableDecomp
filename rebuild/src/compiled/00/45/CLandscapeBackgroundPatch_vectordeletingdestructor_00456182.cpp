#pragma optimize("s",on)
#include <new>
struct CLandscapeBackgroundPatch { int f0; };
extern void __fastcall CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch* self);

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edx_ignored, unsigned int flags)
{
    (void)edx_ignored;
    CLandscapeBackgroundPatch_dtor(self);
    if (flags & 1)
        ::operator delete((void*)self);
    return self;
}