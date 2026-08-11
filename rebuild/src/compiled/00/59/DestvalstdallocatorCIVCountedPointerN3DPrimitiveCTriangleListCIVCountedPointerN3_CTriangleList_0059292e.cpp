#pragma optimize("s",on)
// _Dest_val for std::allocator<CIVCountedPointer<CTriangleList> >
// Intrusive counted-pointer teardown: decrement refcount, virtual-destroy on zero, null the slot.

struct CountedObj {
    virtual void slot0();          // vtable[0]
    virtual void slot1();          // vtable[1] = destroy
    long refcount;                 // +4
};

struct CIVCountedPointer {
    CountedObj* ptr;               // +0
};

// __fastcall: ecx = self (the counted pointer), the trailing allocator/value
// arg pair collapses to the same object here. Model as a free __fastcall on ecx.
void __fastcall _Dest_val(CIVCountedPointer* self, CIVCountedPointer* val)
{
    CountedObj* p = self->ptr;
    if (p) {
        if (--p->refcount == 0) {
            p->slot1();
        }
        self->ptr = 0;
    }
    (void)val;
}