#pragma optimize("s",on)
// Byte-exact reconstruction of _Dest_val<...CIVCountedPointer<CTriangleList>...> at 0x005b1710
// Effectively CIVCountedPointer::Release: decrement refcount, virtual-destroy at zero, null out.

struct Counted {
    void** vtbl;   // +0x00
    int    refCount; // +0x04
};

struct CIVCountedPointer_CTriangleList___ {
    Counted* p; // +0x00
};

// __fastcall models this-in-ecx (single leading pointer param).
void __fastcall CTriangleList___dtor(CIVCountedPointer_CTriangleList___* self)
{
    Counted* c = self->p;
    if (c != 0) {
        if (--c->refCount == 0) {
            // virtual slot 1: [vtbl+4]  -> scalar deleting destructor
            typedef void (__fastcall *Fn)(Counted*);
            ((Fn)(c->vtbl[1]))(c);
        }
        self->p = 0;
    }
}