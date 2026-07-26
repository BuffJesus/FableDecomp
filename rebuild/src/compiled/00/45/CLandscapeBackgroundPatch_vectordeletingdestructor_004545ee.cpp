#pragma optimize("s",on)
extern void __fastcall CLandscapeBackgroundPatch_dtor(void* self);
extern void __cdecl operator_delete(void* p);

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(void* self, int edx_dummy, unsigned int flags)
{
    CLandscapeBackgroundPatch_dtor(self);
    if (flags & 1)
        operator_delete(self);
    return self;
}