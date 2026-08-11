// CAnimComponentStrikeResponseAnim::Clone  (retail 0x005db4a0)
// Virtual clone: allocate a fresh instance via a virtual factory (vtable slot 4),
// then copy the sub-object at +8 from this into the new instance.

struct CAnimComponentBase;

// The sub-object living at offset +8; sub_99efb0 is its member (some copy/assign
// taking a pointer to the source sub-object).
struct SubObj {
    void copyFrom(SubObj* src);   // __fastcall -> harness makes __fastcall (ecx=this)
};

struct CAnimComponentBase {
    // vtable is implicit (first member). Slot 4 (offset 0x10) is the factory.
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual CAnimComponentBase* CreateInstance();   // slot 4 -> [eax+0x10]
    char pad[4];        // bring the sub-object to +8 (vptr=+0, pad=+4)
    SubObj sub;         // at +8
};

struct CAnimComponentStrikeResponseAnim : public CAnimComponentBase {
    CAnimComponentBase* Clone() const;
};

CAnimComponentBase* CAnimComponentStrikeResponseAnim::Clone() const {
    CAnimComponentBase* n = ((CAnimComponentBase*)this)->CreateInstance();
    n->sub.copyFrom(&((CAnimComponentBase*)this)->sub);
    return n;
}