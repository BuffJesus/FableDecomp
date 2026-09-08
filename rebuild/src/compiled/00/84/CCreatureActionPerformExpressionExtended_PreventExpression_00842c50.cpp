#include "engine/CCreatureAction_PerformExpressionExtended.h"
#include "rebuild_abi.h"

extern const char kExprName_00842c50[];
struct CExprField { void Assign(const char *name); };
struct CCreatureAction_PerformExpressionExtended_Methods : CCreatureAction_PerformExpressionExtended {
    void PreventExpression();
};

void CCreatureAction_PerformExpressionExtended_Methods::PreventExpression()
{
    ((CExprField*)((char*)p + 0x78))->Assign(kExprName_00842c50);
}
