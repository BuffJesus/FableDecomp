#include "engine/CConsoleCommandBase.h"

struct CCharString {
    void* p;
    CCharString() {}
    CCharString(const CCharString& o) { p = o.p; }
};

CCharString __fastcall sub_99f570(void* field, const char* lit);

struct CConsoleCommandBase_Methods : CConsoleCommandBase {
    CCharString GetFormattedName() const;
};

CCharString CConsoleCommandBase_Methods::GetFormattedName() const
{
    return sub_99f570((void*)&m_field4, (const char*)0x13caa48);
}
