#pragma optimize("s",on)
// Byte-exact reconstruction of C3DAnimationBlendState::~C3DAnimationBlendState @ 0x004dcdfd

typedef void (__fastcall *ReleaseFn)(void* self);

struct RefCounted {
    ReleaseFn* vtbl;     // +0x00
    int        refcount; // +0x04
};

// Base class whose destructor is the tail-call target @ 0x686830.
struct CBaseAnimState {
    void* base0;         // +0x00
    ~CBaseAnimState();   // non-inline: emitted as external call
};

struct C3DAnimationBlendState : public CBaseAnimState {
    void*       pad4;    // +0x04
    void*       pad8;    // +0x08
    RefCounted* m_c;     // +0x0c
    ~C3DAnimationBlendState();
};

C3DAnimationBlendState::~C3DAnimationBlendState()
{
    RefCounted* c = m_c;
    if (c) {
        if (--c->refcount == 0) {
            c->vtbl[1](c);   // virtual slot 1 (offset +4)
        }
        m_c = 0;
    }
    // implicit base dtor call -> tail-jmp to CBaseAnimState::~CBaseAnimState
}