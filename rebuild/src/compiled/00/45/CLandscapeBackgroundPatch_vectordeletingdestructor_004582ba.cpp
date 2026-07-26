#pragma optimize("s",on)
struct CLandscapeBackgroundPatch;
extern "C" void __fastcall CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch* self);
extern "C" void __cdecl op_delete(void* p);

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edx_unused, unsigned int flags)
{
    (void)edx_unused;
    CLandscapeBackgroundPatch_dtor(self);
    if (flags & 1)
        op_delete((void*)self);
    return (void*)self;
}