// CLandscapeBackgroundPatch::`vector_deleting_destructor' @ 0x0048b600
// Canonical MSVC vector deleting destructor.

struct CLandscapeBackgroundPatch
{
    void scalar_destructor();   // scalar dtor at 0xa0d3d0 (external, non-inlined)
};

extern void __cdecl fable_operator_delete(void*);  // operator delete at 0xbfe9bc

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(
        CLandscapeBackgroundPatch* self, int /*edx*/, unsigned int flags)
{
    self->scalar_destructor();
    if (flags & 1)
        fable_operator_delete(self);
    return self;
}