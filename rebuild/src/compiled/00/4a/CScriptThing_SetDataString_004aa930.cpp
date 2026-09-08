// CScriptThing_Methods::SetDataString  retail 0x004aa930
// CCountedPointer<CScriptThing> forwarder: if the counted pointee (PImp_Data, +0x4)
// is non-null, dispatch its virtual slot 4 ([vtbl+0x10]) with the string argument.
// The header models the pointee as CScriptThing with a plain `void* __vftable`, so the
// virtual dispatch is expressed through a local interface view of the same vtable.

#include "engine/CScriptThing.h"  // retyped onto the PDB layout; byte parity re-verified
struct CCharString;

struct CScriptThingVirtuals {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void SetDataString(const CCharString&);   // [vtbl+0x10]
};

struct CScriptThing_Methods : CScriptThing {
    void SetDataString(const CCharString&);
};

void CScriptThing_Methods::SetDataString(const CCharString& s)
{
    CScriptThing* p = this->PImp_Data;
    if (p)
        ((CScriptThingVirtuals*)p)->SetDataString(s);
}
