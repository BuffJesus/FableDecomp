// CLandscapeBackgroundPatch::`vector_deleting_destructor' 0x0048b4e0
// Standard MSVC vector-deleting destructor thunk.

struct CLandscapeBackgroundPatch
{
    void ScalarDtor();           // 0x00a0d3d0  (the ~destructor body)
};

// operator delete (0x00bfe9bc)
void __cdecl EngineOperatorDelete(void* p);

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(
    CLandscapeBackgroundPatch* self, int /*edx*/, unsigned int flags)
{
    self->ScalarDtor();
    if (flags & 1)
        EngineOperatorDelete(self);
    return self;
}