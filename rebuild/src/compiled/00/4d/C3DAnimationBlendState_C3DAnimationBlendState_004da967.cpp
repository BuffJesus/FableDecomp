#pragma optimize("s",on)
// Byte-exact reconstruction of C3DAnimationBlendState::~C3DAnimationBlendState @ 0x004da967

struct IRefVtbl;

struct IRefCounted {
    IRefVtbl* vtbl;   // +0
    int       refcnt; // +4
};

struct IRefVtbl {
    void (__fastcall *slot0)(IRefCounted*);
    void (__fastcall *slot1)(IRefCounted*);  // +4 : release/destroy
};

// base class whose destructor lives at 0x686830
struct BlendStateBase {
    void* pad0;    // +0
    void* pad4;    // +4
    void* pad8;    // +8
    IRefCounted* ref; // +0xc
    ~BlendStateBase();  // extern, at 0x686830
};

struct C3DAnimationBlendState : public BlendStateBase {
    ~C3DAnimationBlendState();
};

C3DAnimationBlendState::~C3DAnimationBlendState()
{
    IRefCounted* r = this->ref;
    if (r != 0) {
        if (--r->refcnt == 0) {
            r->vtbl->slot1(r);
        }
        this->ref = 0;
    }
}