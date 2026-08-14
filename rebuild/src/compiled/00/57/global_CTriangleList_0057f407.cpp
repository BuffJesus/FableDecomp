#pragma optimize("s",on)
// Reconstruction of _Dest_val destructor for CIVCountedPointer<N3DPrimitive::CTriangleList>
// @ 0x0057f407 -- releases a counted pointer, calling vtable[1] when refcount hits 0.

struct Counted {
    void** vtbl;   // +0: vtable pointer
    long   refs;   // +4: reference count
};

struct CIVCountedPointer {
    Counted* p;    // +0: held pointer

    void release() {
        Counted* c = p;
        if (c) {
            if (--c->refs == 0) {
                // virtual call: vtbl[1]
                ((void (__fastcall*)(Counted*))(c->vtbl[1]))(c);
            }
            p = 0;
        }
    }
};

void __fastcall Dest_val(CIVCountedPointer* self) {
    self->release();
}