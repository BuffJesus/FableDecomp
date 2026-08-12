// CLandscapeBackgroundPatch::`vector_deleting_destructor' 0x0048b680
// __fastcall(this, unsigned int flags) -> void*
#include <stddef.h>

struct CLandscapeBackgroundPatch
{
    void* VectorDeletingDtor(unsigned int flags);
};

// The real (scalar) destructor lives at 0xa0d3d0. It is a separate function;
// retail emits a direct call to it. Declare it extern so the compiler cannot
// inline it and must emit a real call (this in ecx via __fastcall).
extern void __fastcall LandscapeBackgroundPatch_ScalarDtor(CLandscapeBackgroundPatch* self);

void* CLandscapeBackgroundPatch::VectorDeletingDtor(unsigned int flags)
{
    LandscapeBackgroundPatch_ScalarDtor(this);
    if (flags & 1)
        ::operator delete(this);
    return this;
}