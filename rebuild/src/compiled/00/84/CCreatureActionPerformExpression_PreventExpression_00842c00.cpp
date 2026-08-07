#include "rebuild_abi.h"
// CCreatureAction_PerformExpression::PreventExpression @ 0x00842c00
// mov ecx,[ecx+0xa8]; push STR; add ecx,0x78; call ctor; ret
// Assign a fixed string into a CExprField subobject at (this->fa8)+0x78 (thiscall).
extern const char kExprName_00842c00[];
struct CExprField { void Assign(const char *name); };
struct CCreatureAction_PerformExpression { char pad00[0xa8]; char *m_holder; void PreventExpression(); };
void CCreatureAction_PerformExpression::PreventExpression()
{
    ((CExprField*)(m_holder + 0x78))->Assign(kExprName_00842c00);
}