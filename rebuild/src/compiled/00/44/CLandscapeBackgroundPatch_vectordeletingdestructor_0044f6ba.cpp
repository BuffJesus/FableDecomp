#pragma optimize("s",on)
extern void __fastcall CLandscapeBackgroundPatch_dtor(void* self);
extern void __cdecl op_delete(void* p);
extern void* const CLandscapeBackgroundPatch_vftable;

struct CLandscapeBackgroundPatch {
    void* vftable;
};

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edxpad, unsigned int flags)
{
    (void)edxpad;
    self->vftable = (void*)&CLandscapeBackgroundPatch_vftable;
    CLandscapeBackgroundPatch_dtor(self);
    if (flags & 1) {
        op_delete(self);
    }
    return self;
}