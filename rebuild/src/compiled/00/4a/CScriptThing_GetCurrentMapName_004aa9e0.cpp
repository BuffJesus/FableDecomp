#include "engine/CScriptThing.h"  // retyped onto the PDB layout; byte parity re-verified
struct CCharString { char* p; CCharString(const char* s); };
extern const char kDefaultMapName[];
// Vtable model for the CScriptThing pointed to by PImp_Data (header: void* __vftable);
// slot 8 is the virtual name getter. Layout-only: every slot is a plain virtual.
struct CScriptThingVirtuals {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual CCharString getName();
};
struct CScriptThing_Methods : CScriptThing {
    CCharString GetCurrentMapName();
};

CCharString CScriptThing_Methods::GetCurrentMapName() {
    CScriptThingVirtuals* p = (CScriptThingVirtuals*)this->PImp_Data;
    if (p == 0) {
        return CCharString(kDefaultMapName);
    }
    return p->getName();
}
