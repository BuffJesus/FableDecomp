// CLandscapeBackgroundPatch `vector_deleting_destructor'
// retail 0x0048b4a0 __fastcall(this, uint flags) -> void*

// operator delete (retail 0xbfe9bc)
void __cdecl operator_delete_impl(void* p);

struct CLandscapeBackgroundPatch
{
    void destroy();  // real destructor body @ 0xa0d3d0
};

// scalar/vector deleting destructor shape
void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(
        CLandscapeBackgroundPatch* self, int /*edx*/, unsigned int flags)
{
    self->destroy();
    if (flags & 1)
        operator_delete_impl(self);
    return self;
}