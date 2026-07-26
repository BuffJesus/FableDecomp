#pragma optimize("s",on)
// MSVC vector deleting destructor for CLandscapeBackgroundPatch
struct CLandscapeBackgroundPatch {
    void* vftable;
};

extern "C" void __fastcall CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch* self);
extern "C" void __cdecl operator_delete_stub(void* p);

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int /*edx*/, unsigned int flags)
{
    CLandscapeBackgroundPatch_dtor(self);
    if (flags & 1)
        operator_delete_stub(self);
    return self;
}