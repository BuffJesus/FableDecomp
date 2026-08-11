#pragma optimize("s",on)
// Counted object with vtable at +0 and refcount at +4.
struct CountedObj {
    virtual void slot0();   // vtable slot 0
    virtual void slot1();   // vtable slot 1 -> called via [eax+4] when refcount hits 0
    int refcount;           // +4
};

// CIVCountedPointer holds a raw pointer to CountedObj at offset 0.
// _Dest_val destroys the held pointer; this (ecx) = the CIVCountedPointer.
struct CIVCountedPointer {
    CountedObj* p;          // +0
    void Dest_val();
};

void CIVCountedPointer::Dest_val()
{
    CountedObj* c = this->p;
    if (c) {
        if (--c->refcount == 0) {
            c->slot1();
        }
        this->p = 0;
    }
}