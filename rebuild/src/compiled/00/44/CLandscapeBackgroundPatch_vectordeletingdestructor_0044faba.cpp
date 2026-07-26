#pragma optimize("s",on)
struct CLandscapeBackgroundPatch {
    void* vptr;
};
extern void __fastcall CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch* self);
extern void __cdecl engine_delete(void* p);
extern void* const CLandscapeBackgroundPatch_vftable;

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int /*edx*/, unsigned int flags)
{
    self->vptr = (void*)&CLandscapeBackgroundPatch_vftable;
    CLandscapeBackgroundPatch_dtor(self);
    if (flags & 1) {
        engine_delete(self);
    }
    return self;
}