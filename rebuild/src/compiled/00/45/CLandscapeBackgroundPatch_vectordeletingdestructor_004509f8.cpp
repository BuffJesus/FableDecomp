#pragma optimize("s",on)
struct CInner { int a; };
struct CLandscapeBackgroundPatch {
    int f0;
    int f4;
    void* f8;
    CInner inner;
    unsigned char _pad_0x10[0xa8];
    void* tex;
};

extern void __fastcall CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch* self);
extern void __cdecl operator_delete(void* p);

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edx, unsigned int flags)
{
    (void)edx;
    CLandscapeBackgroundPatch_dtor(self);
    if (flags & 1) {
        operator_delete(self);
    }
    return self;
}