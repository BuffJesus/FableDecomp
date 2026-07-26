extern void __cdecl operator_delete_stub(void*);
struct CLandscapeBackgroundPatch { void* vftable; };
extern const void* CLandscapeBackgroundPatch_vftable;
extern void __fastcall CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch* self);
void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int _edx, unsigned int flags)
{
    self->vftable = (void*)&CLandscapeBackgroundPatch_vftable;
    CLandscapeBackgroundPatch_dtor(self);
    if (flags & 1) {
        operator_delete_stub(self);
    }
    return self;
}