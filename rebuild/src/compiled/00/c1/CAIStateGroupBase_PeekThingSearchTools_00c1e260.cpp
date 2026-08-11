// CAIStateGroupBase::PeekThingSearchTools  @ 0x00c1e260
// __fastcall, returns CThingSearchTools*

struct CThingSearchTools;

// The callee is a subobject at (field4->field4) + 0x50. Its vptr is at its own
// offset 0; we call vtable slot 8 (0x20), passing the outer 'this' as the arg.
struct Sub {
    virtual void  s0();
    virtual void  s1();
    virtual void  s2();
    virtual void  s3();
    virtual void  s4();
    virtual void  s5();
    virtual void  s6();
    virtual void  s7();
    virtual void* slot8(void* arg);  // slot at 0x20
};

struct Inner {
    char pad50[0x50];  // +0x00 .. +0x4f
    Sub  sub;          // +0x50
};

struct Field4 {
    void*  pad;        // +0x00
    Inner* inner;      // +0x04  (field4->field4)
};

// vtable literal written into this->vptr
extern void* const s_vtbl_12b33f0[];

struct CAIStateGroupBase {
    void** vptr;       // +0x00
    Field4* field4;    // +0x04
    CThingSearchTools* PeekThingSearchTools();
};

CThingSearchTools* CAIStateGroupBase::PeekThingSearchTools()
{
    Field4* f = this->field4;
    this->vptr = (void**)s_vtbl_12b33f0;
    return (CThingSearchTools*)f->inner->sub.slot8((void*)this);
}