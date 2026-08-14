#pragma optimize("s",on)
// Byte-exact reconstruction of C3DAnimationBlendState::~C3DAnimationBlendState @ 0x004db004
// Real member dtor so the emitted symbol contains "~C3DAnimationBlendState".

struct RefCounted {
    void** vtbl;   // +0x00
    int    refs;   // +0x04
};

// base class destructor tail target @ 0x686830
struct CBase3DAnimState {
    char pad[0x14];
    CBase3DAnimState();
    ~CBase3DAnimState();
};

struct C3DAnimationBlendState : CBase3DAnimState {
    // base occupies +0x00..+0x13
    RefCounted* ptr;   // +0x14
    ~C3DAnimationBlendState();
};

C3DAnimationBlendState::~C3DAnimationBlendState()
{
    RefCounted* p = this->ptr;
    if (p) {
        if (--p->refs == 0) {
            typedef void (__fastcall *VF)(RefCounted*);
            ((VF)p->vtbl[1])(p);
        }
        this->ptr = 0;
    }
    // base dtor runs here (tail-call jmp 0x686830)
}