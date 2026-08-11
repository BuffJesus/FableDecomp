// CIsThingAlive::operator() - retail 0x00c2ed50
// A predicate functor: calls virtual slot 5 on itself (discard),
// on embedded object at +0x10 (discard), then tail-returns virtual
// slot 5 on embedded object at +0x20.

struct IPolyA {
    virtual int va0();
    virtual int va1();
    virtual int va2();
    virtual int va3();
    virtual int va4();
    virtual bool va5();   // slot 5 = vtable offset 0x14
};

struct IPolyB {
    virtual int vb0();
    virtual int vb1();
    virtual int vb2();
    virtual int vb3();
    virtual int vb4();
    virtual bool vb5();
};

struct IPolyC {
    virtual int vc0();
    virtual int vc1();
    virtual int vc2();
    virtual int vc3();
    virtual int vc4();
    virtual bool vc5();
};

struct CIsThingAlive : public IPolyA {
    // vtable ptr at +0x00 (from IPolyA base)
    char pad[12];       // fill up to +0x10
    IPolyB objB;        // at +0x10 (vtable ptr at its +0x00)
    char padB[12];      // fill B's body up to +0x20
    IPolyC objC;        // at +0x20
    bool operatorCall();
};

bool CIsThingAlive::operatorCall()
{
    this->va5();
    this->objB.vb5();
    return this->objC.vc5();
}