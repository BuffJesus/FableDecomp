#pragma optimize("s",on)
// Byte-exact reconstruction of C3DAnimationBlendState::~C3DAnimationBlendState @ 0x004dbf56

struct RefVtbl;
struct RefCounted {
    RefVtbl* vtbl;   // +0
    int      refs;   // +4
};
struct RefVtbl {
    void (__fastcall *slot0)(RefCounted*);
    void (__fastcall *slot1)(RefCounted*);   // +4 : release/destroy
};

// Base class whose destructor lives at 0x686830 (tail-jmp target).
struct BlendBase {
    char pad0[0x14];       // +0x00
    RefCounted* ptr;       // +0x14
    ~BlendBase();          // 0x686830
};

struct C3DAnimationBlendState : public BlendBase {
    ~C3DAnimationBlendState();
};

C3DAnimationBlendState::~C3DAnimationBlendState()
{
    RefCounted* p = this->ptr;
    if (p) {
        if (--p->refs == 0) {
            p->vtbl->slot1(p);
        }
        this->ptr = 0;
    }
    // implicit chain to ~BlendBase() at 0x686830 -> tail jmp
}