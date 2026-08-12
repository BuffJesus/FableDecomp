#pragma optimize("s",on)
// Counted object: vtable at +0, refcount at +4.
struct CCounted {
    virtual void f0();          // slot 0
    virtual void destroy();     // slot 1  -> call [eax+4]
    int refcount;               // +4
};

// The counted pointer holds CCounted* at +0.
struct CIVCountedPointer {
    CCounted* p;                // +0
    void _Dest_val();
};

void CIVCountedPointer::_Dest_val()
{
    CCounted* c = this->p;      // ecx = [esi]
    if (c != 0) {
        if (--c->refcount == 0) {
            c->destroy();       // call [eax+4]
        }
        this->p = 0;            // and [esi], 0
    }
}