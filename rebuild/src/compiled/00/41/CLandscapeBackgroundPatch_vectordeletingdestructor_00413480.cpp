struct CInner;
void __fastcall CLandscapeBackgroundPatch_dtor_inner(CInner* self);
extern void __cdecl engine_operator_delete(void* p);

struct CLandscapeBackgroundPatch {
    unsigned int vftable;
    char inner[0xb4];
    void* tex;
};

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, void* edx, unsigned int flags)
{
    (void)edx;
    self->vftable = 0x122e5b0;
    CLandscapeBackgroundPatch_dtor_inner((CInner*)(&self->inner));
    if (flags & 1)
        engine_operator_delete(self);
    return self;
}