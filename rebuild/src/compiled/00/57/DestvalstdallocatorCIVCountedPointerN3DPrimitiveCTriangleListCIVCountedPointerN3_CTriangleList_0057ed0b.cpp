#pragma optimize("s",on)
// Byte-exact reconstruction of _Dest_val<allocator<CIVCountedPointer<CTriangleList>>, CIVCountedPointer<CTriangleList>*>
// retail 0x0057ed0b
//
// The counted object: vtable ptr at +0, refcount (int) at +4.
// A virtual through slot 1 (vtable+4) is the destroy/release call.

struct CTriangleList {
    virtual void slot0();      // slot0 (vtable+0)
    virtual void destroy();    // slot1 (vtable+4)  -- called on last release
    int refcount;              // +4
};

struct CIVCountedPointer {
    CTriangleList* ptr;        // +0
};

// __fastcall: self (the counted pointer holder) arrives in ecx.
// (second allocator/edx arg unused by codegen)
void __fastcall Dest_val(CIVCountedPointer* self)
{
    CTriangleList* p = self->ptr;
    if (p != 0) {
        if (--p->refcount == 0) {
            p->destroy();
        }
        self->ptr = 0;
    }
}