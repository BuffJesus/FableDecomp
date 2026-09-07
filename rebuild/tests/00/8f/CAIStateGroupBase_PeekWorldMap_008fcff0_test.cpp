#include "engine/CAIStateGroupBase.h"
#include <cstdio>

struct CWorldMap { int tag; };

struct CInner {
    virtual CWorldMap* Peek0(){return 0;}
    virtual CWorldMap* Peek1(){return 0;}
    virtual CWorldMap* Peek2(){return 0;}
    virtual CWorldMap* Peek3(){return 0;}
    virtual CWorldMap* Peek4(){return 0;}
    virtual CWorldMap* Peek5(){return 0;}
    virtual CWorldMap* Peek6(){return 0;}
    virtual CWorldMap* Peek7(){return 0;}
    virtual CWorldMap* Peek8(){return 0;}
    virtual CWorldMap* Peek9(){return 0;}
    virtual CWorldMap* Peek10(){return 0;}
    virtual CWorldMap* Peek11(){return 0;}
    virtual CWorldMap* Peek12(){return 0;}
    virtual CWorldMap* PeekWorldMap();
};

struct CMid { char pad[0x24]; CInner* inner; };
struct CAIBrain { char pad[0x1c]; CMid* mid; };

static CWorldMap g_wm = { 0xABCD };

struct CInnerImpl : CInner {
    virtual CWorldMap* PeekWorldMap() { return &g_wm; }
};
CWorldMap* CInner::PeekWorldMap() { return 0; }

CWorldMap* __fastcall CAIStateGroupBase_PeekWorldMap(CAIStateGroupBase* self)
{
    return self->PBrain->mid->inner->PeekWorldMap();
}

int main() {
    CInnerImpl innerObj;
    CMid midObj; midObj.inner = &innerObj;
    CAIBrain holderObj; holderObj.mid = &midObj;
    CAIStateGroupBase baseObj; baseObj.PBrain = &holderObj;
    CWorldMap* r = CAIStateGroupBase_PeekWorldMap(&baseObj);
    if (r == &g_wm && r->tag == 0xABCD) {
        std::printf("CAIStateGroupBase_008fcff0_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}