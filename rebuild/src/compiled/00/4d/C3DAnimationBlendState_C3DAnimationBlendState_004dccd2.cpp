#pragma optimize("s",on)
struct RefCounted;
struct RefCountedVtbl {
    void (__fastcall *dtor)(RefCounted*);
    void (__fastcall *release)(RefCounted*);
};
struct RefCounted {
    RefCountedVtbl* vtbl;
    int refcount;
};

// non-trivial base destructor defined out-of-line (address 0x686830)
struct C3DAnimBlendBase {
    char basepad[0xc];
    ~C3DAnimBlendBase();
};

struct C3DAnimationBlendState : public C3DAnimBlendBase {
    RefCounted* ptr;   // lands at +0xc (base is 0xc bytes)
    ~C3DAnimationBlendState();
};

C3DAnimationBlendState::~C3DAnimationBlendState()
{
    RefCounted* p = ptr;
    if (p) {
        if (--p->refcount == 0) {
            p->vtbl->release(p);
        }
        ptr = 0;
    }
    // base ~C3DAnimBlendBase() runs after body -> tail jmp
}