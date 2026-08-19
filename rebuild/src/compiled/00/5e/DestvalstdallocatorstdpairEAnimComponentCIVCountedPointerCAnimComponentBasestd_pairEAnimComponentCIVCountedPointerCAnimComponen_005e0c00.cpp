// Retail 0x009b20b0: destroy helper for
// pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase>>.
// ecx = self; [self+4] holds a counted-pointer. On non-null: decrement
// the pointee's refcount at [p+4]; if it reaches zero, virtual-call
// vtable slot 1 (delete-this); then null the stored pointer.

struct CAnimComponentBase
{
    virtual void v0();          // slot 0
    virtual void Release();     // slot 1  ([vtable+4])
    int m_refCount;             // +4
};

struct DestPair
{
    int          m_first;       // +0  EAnimComponent
    CAnimComponentBase* m_ptr;  // +4  CIVCountedPointer payload

    void _Dest_val();
};

void DestPair::_Dest_val()
{
    CAnimComponentBase* p = m_ptr;
    if (p != 0)
    {
        if (--p->m_refCount == 0)
        {
            p->Release();
        }
        m_ptr = 0;
    }
}