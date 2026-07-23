#pragma optimize("s",on)
extern const void* CLandscapeBackgroundPatch_vftable;
extern "C" void __cdecl FableFreeMem(void* p);

struct CLandscapeBackgroundPatch {
    const void** vft;
};

void __fastcall CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch* self);

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edx, unsigned int flags) {
    (void)edx;
    self->vft = (const void**)&CLandscapeBackgroundPatch_vftable;
    CLandscapeBackgroundPatch_dtor(self);
    if (flags & 1) {
        FableFreeMem(self);
    }
    return self;
}