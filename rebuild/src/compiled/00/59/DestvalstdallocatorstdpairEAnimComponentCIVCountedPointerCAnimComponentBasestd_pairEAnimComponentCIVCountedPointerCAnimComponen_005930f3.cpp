#pragma optimize("s",on)
// _Dest_val for std::allocator<std::pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase>>>
// Destroys the pair. first member (EAnimComponent enum) is trivial; the second member is a
// CIVCountedPointer whose held CAnimComponentBase sits at +4. Releasing it: if inner != 0,
// --inner->refCount; if it hit 0, call virtual slot 1 (scalar-deleting dtor at vtable[+4]),
// then null the inner pointer.

struct CAnimComponentBase {
    virtual void slot0();          // vtable[+0]
    virtual void Release();        // vtable[+4]  slot 1 -- called on refcount==0
    int refCount;                  // +4
};

struct DestPair {
    int                 animComponent; // +0  EAnimComponent
    CAnimComponentBase* inner;         // +4  held ptr
    void Dest_val();
};

void DestPair::Dest_val()
{
    CAnimComponentBase* p = this->inner;
    if (p != 0) {
        if (--p->refCount == 0) {
            p->Release();
        }
        this->inner = 0;
    }
}