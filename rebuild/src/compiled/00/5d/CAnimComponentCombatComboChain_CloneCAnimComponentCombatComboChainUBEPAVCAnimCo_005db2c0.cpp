struct T {
    void* vtbl_slot0;
    void* f4;
    void* f8;
    void* fc;
    // vtable is at *(void***)this ; slot at +0x10 = index 4
};

struct VT {
    void* s0;
    void* s1;
    void* s2;
    void* s3;
    T* (__fastcall* clone)(void* self); // slot 4, [eax+0x10]
};

T* __fastcall Clone_ComboChain(T* self)
{
    VT* vt = *(VT**)self;
    T* n = vt->clone(self);
    n->fc = self->fc;
    n->f8 = self->f8;
    return n;
}