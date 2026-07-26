#pragma optimize("s",on)
struct CLandscapeBackgroundPatch {
    int f0;
    int f4;
    void* f8;
    char inner[4];
    unsigned char _pad_0x10[0xa8];
    void* tex;
    ~CLandscapeBackgroundPatch();
};
extern "C" void __cdecl fable_operator_delete(void*);

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edx, unsigned int flags) {
    self->~CLandscapeBackgroundPatch();
    if (flags & 1) {
        fable_operator_delete(self);
    }
    return self;
}