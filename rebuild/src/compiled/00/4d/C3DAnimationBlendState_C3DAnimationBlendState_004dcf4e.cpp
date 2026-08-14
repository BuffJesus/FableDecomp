#pragma optimize("s",on)
// ~C3DAnimationBlendState @ 0x004dcf4e
struct IRefCounted {
    struct VTable { void (__fastcall *slot0)(void*); void (__fastcall *slot1)(void*); };
    VTable* vtbl;
    int     refcount;
};
struct Base {
    void* basefield;
    ~Base();
};
struct C3DAnimationBlendState : public Base {
    char pad0[8];
    IRefCounted* res;
    ~C3DAnimationBlendState();
};
C3DAnimationBlendState::~C3DAnimationBlendState()
{
    IRefCounted* r = this->res;
    if (r) {
        if (--r->refcount == 0) {
            r->vtbl->slot1(r);
        }
        this->res = 0;
    }
}