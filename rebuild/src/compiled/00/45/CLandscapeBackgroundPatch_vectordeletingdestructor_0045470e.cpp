#pragma optimize("s",on)
struct CMember { };
struct CLandscapeBackgroundPatch {
    void* vtbl;
    int f4;
    void* f8;
    unsigned char pad[0x74 - 0xc];
    CMember member;
    void* CLandscapeBackgroundPatch_vector_deleting_destructor(unsigned char flags);
};

extern void __fastcall CMember_dtor(CMember* self);
extern void __fastcall CLandscapeBackgroundPatch_base_dtor(CLandscapeBackgroundPatch* self);
extern void* g_CLandscapeBackgroundPatch_vftable;
extern void __cdecl operator_delete(void* p);

void* CLandscapeBackgroundPatch::CLandscapeBackgroundPatch_vector_deleting_destructor(unsigned char flags)
{
    CMember_dtor(&this->member);
    this->vtbl = &g_CLandscapeBackgroundPatch_vftable;
    CLandscapeBackgroundPatch_base_dtor(this);
    if (flags & 1) {
        operator_delete(this);
    }
    return this;
}