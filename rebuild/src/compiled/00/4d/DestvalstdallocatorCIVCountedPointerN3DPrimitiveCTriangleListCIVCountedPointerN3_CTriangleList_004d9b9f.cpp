#pragma optimize("s",on)
// Counted-body: has a vtable and a refcount at +4.
struct CountedBody {
    virtual void slot0();          // vtable+0
    virtual void slot1_destroy();  // vtable+4
    int refcount;                  // +4
};

// CIVCountedPointer holds a pointer to the counted body at +0.
struct CIVCountedPointer {
    CountedBody* ptr;              // +0
};

// _Dest_val: release the counted pointer. Signature carries an allocator*
// as the first arg (ecx=this here is the CIVCountedPointer being destroyed).
void __fastcall Dest_val_release(CIVCountedPointer* self)
{
    CountedBody* body = self->ptr;
    if (body != 0) {
        if (--body->refcount == 0) {
            body->slot1_destroy();
        }
        self->ptr = 0;
    }
}