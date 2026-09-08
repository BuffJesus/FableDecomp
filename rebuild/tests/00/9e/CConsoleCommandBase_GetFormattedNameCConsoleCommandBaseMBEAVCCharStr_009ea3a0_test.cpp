#include "engine/CConsoleCommandBase.h"
#include <cstdio>

struct CCharString {
    void* p;
    CCharString() {}
    CCharString(const CCharString& o) { p = o.p; }
};

struct CConsoleCommandBase_Methods : CConsoleCommandBase {
    CCharString GetFormattedName() const;
};

static void* g_lastField;
static const char* g_lastLit;
CCharString __fastcall sub_99f570(void* field, const char* lit)
{
    g_lastField = field; g_lastLit = lit;
    CCharString r; r.p = (void*)0xABCD1234; return r;
}

int main()
{
    CConsoleCommandBase_Methods obj;
    obj.__vftable = 0; obj.m_field4 = 7;
    CCharString r = obj.GetFormattedName();
    bool ok = g_lastField == (void*)&obj.m_field4 &&
              g_lastLit == (const char*)0x13caa48 && r.p == (void*)0xABCD1234;
    if (ok) printf("GETFMTNAME_OK\n");
    else printf("GETFMTNAME_FAIL field=%p lit=%p p=%p\n", g_lastField, g_lastLit, r.p);
    return ok ? 0 : 1;
}
