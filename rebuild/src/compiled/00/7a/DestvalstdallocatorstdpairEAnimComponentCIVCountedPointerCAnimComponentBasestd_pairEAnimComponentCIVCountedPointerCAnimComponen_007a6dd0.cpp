// Counted object: vtable ptr at +0, refcount at +4
struct CAnimComponentBase {
    virtual void v0() = 0;
    virtual void v1() = 0;  // slot 1 = [vtbl+4]
    int refcount;       // +4
};

// The pair holds a counted pointer at offset 0x18.
struct DestPair {
    char pad[0x18];
    CAnimComponentBase* p;  // +0x18

    void Dest_val();
};

void DestPair::Dest_val()
{
    CAnimComponentBase* c = this->p;
    if (c) {
        if (--c->refcount == 0) {
            c->v1();
        }
        this->p = 0;
    }
}