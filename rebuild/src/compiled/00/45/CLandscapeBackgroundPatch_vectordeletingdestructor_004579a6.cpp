#pragma optimize("s",on)
struct CLandscapeBackgroundPatch { void* vftable; };
extern "C" void __fastcall CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch* self);
extern "C" void __cdecl fable_operator_delete(void* p);

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edx_unused, unsigned int flags)
{
    CLandscapeBackgroundPatch_dtor(self);
    if (flags & 1)
        fable_operator_delete(self);
    return self;
}