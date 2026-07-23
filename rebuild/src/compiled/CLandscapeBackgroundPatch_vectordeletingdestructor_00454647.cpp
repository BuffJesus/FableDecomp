#pragma optimize("s",on)
struct CMember { };
extern void __fastcall CMember_dtor(CMember* self);
extern void __fastcall Base_dtor(void* self);
extern void __cdecl op_delete(void* p);
extern const int g_vtbl;

struct CLandscapeBackgroundPatch {
    int f0;
    int f4;
    void* f8;
    char pad[0x28 - 0xc];
    CMember m28;
};

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edx, unsigned char flags)
{
    CMember_dtor(&self->m28);
    *(const int**)self = &g_vtbl;
    Base_dtor(self);
    if (flags & 1)
        op_delete(self);
    return self;
}