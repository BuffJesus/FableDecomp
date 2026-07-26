#pragma optimize("s",on)
struct CLandscapeBackgroundPatch { void* vftable; };

extern void __fastcall CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch* self);
extern void __cdecl eng_operator_delete(void* p);

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edx, unsigned int flags) {
    (void)edx;
    CLandscapeBackgroundPatch_dtor(self);
    if (flags & 1) {
        eng_operator_delete(self);
    }
    return self;
}