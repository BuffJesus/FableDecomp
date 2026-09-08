#include <cstdio>
#include <cstring>
#include "engine/CScriptThing.h"
struct CCharString { char* p; CCharString(const char* s) { p = (char*)s; } CCharString() { p = 0; } };
char g_default[] = "DefaultMap";
const char* kDefaultMapName = g_default;
char g_named[] = "NamedMap";
struct CScriptThingVirtuals {
    virtual void v0() {}
    virtual void v1() {}
    virtual void v2() {}
    virtual void v3() {}
    virtual void v4() {}
    virtual void v5() {}
    virtual void v6() {}
    virtual void v7() {}
    virtual CCharString getName() { return CCharString(g_named); }
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

int main() {
    CScriptThing_Methods a; a.PImp_Data = 0;
    CCharString r1 = a.GetCurrentMapName();
    if (std::strcmp(r1.p, "DefaultMap") != 0) { std::printf("FAIL null path\n"); return 1; }
    CScriptThingVirtuals node;
    CScriptThing_Methods b; b.PImp_Data = (CScriptThing*)&node;
    CCharString r2 = b.GetCurrentMapName();
    if (std::strcmp(r2.p, "NamedMap") != 0) { std::printf("FAIL nonnull path\n"); return 1; }
    std::printf("CScriptThing_004aa9e0_TEST PASS\n");
    return 0;
}
