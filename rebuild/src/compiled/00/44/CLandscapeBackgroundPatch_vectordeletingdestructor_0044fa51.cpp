#pragma optimize("s",on)
extern "C" void __fastcall CLandscapeBackgroundPatch_dtor(void* self);
extern "C" void __cdecl fable_operator_delete(void* p);

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(void* self, int edx, unsigned int flags)
{
    (void)edx;
    CLandscapeBackgroundPatch_dtor(self);
    if (flags & 1)
        fable_operator_delete(self);
    return self;
}