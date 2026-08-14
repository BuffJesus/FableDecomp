#pragma optimize("s",on)
// Byte-exact reconstruction of C3DAnimationBlendState::~C3DAnimationBlendState @ 0x004db47d

// Refcounted control block: vtable at +0, refcount at +4.
struct RefBlock {
    void** vtbl;
    int refcount;
};

// Base class whose dtor we tail-call.
struct C3DAnimationBlendStateBase {
    // dummy to give it a dtor target
};

// The blend state layout: we only care about the shared-ptr slot at +0x10.
struct C3DAnimationBlendState {
    char pad[0x10];
    RefBlock* ptr; // +0x10
};

// Base destructor at 0x686830 (external). __fastcall so this ptr is in ecx.
extern "C" void __fastcall C3DAnimationBlendStateBase_dtor(C3DAnimationBlendState* self);

void __fastcall dtor_C3DAnimationBlendState(C3DAnimationBlendState* self)
{
    RefBlock* p = self->ptr;
    if (p) {
        if (--p->refcount == 0) {
            // virtual slot [vtbl+4]
            ((void(__fastcall*)(RefBlock*))p->vtbl[1])(p);
        }
        self->ptr = 0;
    }
    C3DAnimationBlendStateBase_dtor(self);
}