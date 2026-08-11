// CIsThingAlive::operator() @ 00c2ed70
// __fastcall bool operator()(CIsThingAlive* this)
//
// this has an own vtable at +0, a polymorphic member A at +0x10,
// and a polymorphic member B at +0x20. Each call is virtual slot 5.

struct IPolyA {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual int  v5();   // slot 5 -> [vtbl+0x14]
};

struct IPolyB {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual bool v5();   // slot 5 -> [vtbl+0x14]
};

struct CIsThingAlive {
    // own vtable lives at +0 via the virtual below
    virtual void s0();
    virtual void s1();
    virtual void s2();
    virtual void s3();
    virtual void s4();
    virtual void s5();   // slot 5, own vtable call, result discarded
    // +4 .. +0xC padding fields (occupied by base data)
    int pad0;            // +0x04
    int pad1;            // +0x08
    int pad2;            // +0x0C
    IPolyA a;            // +0x10 (vtable at +0x10)
    int padA1;           // +0x14
    int padA2;           // +0x18
    int padA3;           // +0x1C
    IPolyB b;            // +0x20 (vtable at +0x20)

    bool operator()();
};

bool CIsThingAlive::operator()()
{
    this->s5();
    this->a.v5();
    return this->b.v5();
}