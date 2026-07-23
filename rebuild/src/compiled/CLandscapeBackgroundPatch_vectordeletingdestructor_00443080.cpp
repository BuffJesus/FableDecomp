struct CLandscapeBackgroundPatch {
    void* vftable;
    void* CLandscapeBackgroundPatch_vector_deleting_destructor(unsigned int flags);
};
extern "C" void __fastcall CLandscapeBackgroundPatch_scalar_dtor(CLandscapeBackgroundPatch* self);
extern "C" void __cdecl fable_operator_delete(void* p);

void* CLandscapeBackgroundPatch::CLandscapeBackgroundPatch_vector_deleting_destructor(unsigned int flags)
{
    CLandscapeBackgroundPatch_scalar_dtor(this);
    if (flags & 1)
        fable_operator_delete((void*)this);
    return (void*)this;
}