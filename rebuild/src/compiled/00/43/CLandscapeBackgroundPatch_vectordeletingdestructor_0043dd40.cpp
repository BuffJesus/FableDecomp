struct CLandscapeBackgroundPatch { void* vftable; };
extern "C" void __fastcall CLandscapeBackgroundPatch_scalar_dtor(CLandscapeBackgroundPatch* self);
extern "C" void __cdecl CLandscapeBackgroundPatch_op_delete(void* p);

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edx_dummy, unsigned int flags)
{
    (void)edx_dummy;
    CLandscapeBackgroundPatch_scalar_dtor(self);
    if (flags & 1)
        CLandscapeBackgroundPatch_op_delete(self);
    return self;
}