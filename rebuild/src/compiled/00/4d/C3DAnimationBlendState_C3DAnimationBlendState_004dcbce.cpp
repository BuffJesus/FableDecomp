#pragma optimize("s",on)
// Byte-exact reconstruction of C3DAnimationBlendState::~C3DAnimationBlendState @ 0x004dcbce

struct RefCounted {
    void** vtbl;    // +0x00
    long   refCnt;  // +0x04
};

// Base class with a destructor at 0x00686830 (tail-jmp target).
struct Base {
    void* vtbl;
    ~Base();   // extern; defined as 0x00686830
};

struct C3DAnimationBlendState : public Base {
    // Base occupies +0x00 (vtbl). rc at +0x10.
    char pad[0x0C];
    RefCounted* rc;   // +0x10
    ~C3DAnimationBlendState();
};

C3DAnimationBlendState::~C3DAnimationBlendState()
{
    RefCounted* rc = this->rc;
    if (rc)
    {
        if (--rc->refCnt == 0)
        {
            typedef void (__fastcall *Fn)(RefCounted*);
            ((Fn)(rc->vtbl[1]))(rc);
        }
        this->rc = 0;
    }
    // base dtor runs implicitly -> tail-jmp to 0x00686830
}