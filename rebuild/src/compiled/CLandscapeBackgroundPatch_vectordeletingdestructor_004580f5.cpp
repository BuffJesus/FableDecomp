#pragma optimize("s",on)
struct CLandscapeBackgroundPatch { void* vftable; };
extern void __fastcall CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch* self);
extern void __cdecl operator_delete_stub(void* p);

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edx_unused, unsigned int flags)
{
    CLandscapeBackgroundPatch_dtor(self);
    if (flags & 1)
        operator_delete_stub(self);
    return self;
}