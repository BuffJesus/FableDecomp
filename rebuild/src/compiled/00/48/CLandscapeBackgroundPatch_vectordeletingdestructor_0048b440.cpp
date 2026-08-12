// CLandscapeBackgroundPatch::`vector_deleting_destructor'
// Standard MSVC vector deleting destructor shape.

struct CLandscapeBackgroundPatch
{
    void dtor();  // scalar destructor at 0xa0d3d0
};

extern void __cdecl fable_operator_delete(void* p);  // 0xbfe9bc

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(
    CLandscapeBackgroundPatch* self, int /*edx*/, unsigned int flags)
{
    self->dtor();
    if (flags & 1)
        fable_operator_delete(self);
    return self;
}