#include "engine/CCreatureAction_PerformExpression.h"
#include "rebuild_abi.h"

extern const char kExprName_00842c00[];
struct CExprField { void Assign(const char *name); };
struct CCreatureAction_PerformExpression_Methods : CCreatureAction_PerformExpression {
    void PreventExpression();
};

void CCreatureAction_PerformExpression_Methods::PreventExpression()
{
    ((CExprField*)((char*)p + 0x78))->Assign(kExprName_00842c00);
}
