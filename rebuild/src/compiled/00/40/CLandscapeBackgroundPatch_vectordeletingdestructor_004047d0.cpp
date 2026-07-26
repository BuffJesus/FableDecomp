struct CInner { void* v; };
extern void __fastcall Inner14_dtor(void* p);
extern void __fastcall Inner_dtor(void* p);
extern void __fastcall CLBP_dtor(void* p);
extern void __cdecl operator_delete_stub(void* p);
void operator delete(void* p);

struct CLandscapeBackgroundPatch {
    void* vftable;
    CInner inner4;
    void* f8;
    CInner innerC;
    unsigned char _pad_0x10[0xa8];
    void* tex;
};

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edx_dummy, unsigned int flags)
{
    Inner14_dtor((char*)self + 0x14);
    Inner_dtor((char*)self + 0x10);
    Inner_dtor((char*)self + 0xc);
    CLBP_dtor(self);
    if (flags & 1)
        operator delete(self);
    return self;
}