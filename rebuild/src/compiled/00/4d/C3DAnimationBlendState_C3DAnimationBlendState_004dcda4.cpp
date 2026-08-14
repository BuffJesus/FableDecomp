#pragma optimize("s",on)
struct RefCounted {
    void** vtbl;
    int    refc;
};

struct C3DAnimationBlendState {
    char        pad[0x14];
    RefCounted* pRef;
};

void __fastcall base_dtor(void* self);

void __fastcall dtor_C3DAnimationBlendState(C3DAnimationBlendState* self)
{
    RefCounted* p = self->pRef;
    if (p != 0) {
        if (--p->refc == 0) {
            typedef void (__fastcall *fn_t)(RefCounted*);
            ((fn_t)(p->vtbl[1]))(p);
        }
        self->pRef = 0;
    }
    base_dtor(self);
}