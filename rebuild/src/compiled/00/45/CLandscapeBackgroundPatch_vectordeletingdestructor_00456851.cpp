#pragma optimize("s",on)
extern void __fastcall CLandscapeBackgroundPatch_dtor(void* self);
extern void __cdecl op_delete(void* p);

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(void* self, int /*edx*/, unsigned int flags)
{
    CLandscapeBackgroundPatch_dtor(self);
    if (flags & 1)
        op_delete(self);
    return self;
}