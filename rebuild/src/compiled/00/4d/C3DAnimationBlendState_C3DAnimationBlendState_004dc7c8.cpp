// Byte-exact reconstruction of C3DAnimationBlendState::~C3DAnimationBlendState @ 0x004dc7c8

struct RefObj;
struct RefVtbl {
    void (__fastcall *dtor_slot0)(RefObj*);
    void (__fastcall *release_slot1)(RefObj*);
};
struct RefObj {
    RefVtbl* vtbl;   // +0
    int      count;  // +4
};

// base class @ 0x686830 dtor; occupies [0..0xc)
struct BlendBase {
    char pad0[0xc];
    ~BlendBase();
};

struct C3DAnimationBlendState : public BlendBase {
    // ptr at +0xc
    RefObj* ptr;
    ~C3DAnimationBlendState();
};

#pragma optimize("s", on)
C3DAnimationBlendState::~C3DAnimationBlendState()
{
    RefObj* p = this->ptr;
    if (p) {
        if (--p->count == 0) {
            p->vtbl->release_slot1(p);
        }
        this->ptr = 0;
    }
    // base dtor runs here as tail-call
}