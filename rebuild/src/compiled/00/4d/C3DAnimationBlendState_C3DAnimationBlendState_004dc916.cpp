#pragma optimize("s",on)
// Byte-exact reconstruction of C3DAnimationBlendState::~C3DAnimationBlendState @ 0x004dc916

struct RefCounted {
    void** vtable;   // +0
    int    refCount; // +4
};

struct BaseBlendState;
// base destructor tail-called at 0x686830
extern "C" void __fastcall Base_dtor(BaseBlendState* self);

struct C3DAnimationBlendState {
    char        pad0[0xc];   // +0
    RefCounted* handle;      // +0xc
};

void __fastcall dtor(C3DAnimationBlendState* self)
{
    RefCounted* h = self->handle;
    if (h) {
        if (--h->refCount == 0) {
            // virtual slot at [vtable+4]
            typedef void (__fastcall *Fn)(RefCounted*);
            ((Fn)h->vtable[1])(h);
        }
        self->handle = 0;
    }
    Base_dtor((BaseBlendState*)self);
}