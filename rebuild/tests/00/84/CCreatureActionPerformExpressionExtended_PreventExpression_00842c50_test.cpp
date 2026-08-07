#include "rebuild_abi.h"
#include <cstdio>
const char kExprName_00842c50[] = "expr";
static void *g_dst_00842c50=0; static const char *g_name_00842c50=0;
struct CExprField { void Assign(const char *name){ g_dst_00842c50=this; g_name_00842c50=name; } };
struct CCreatureAction_PerformExpressionExtended { char pad00[0xa8]; char *m_holder; void PreventExpression(); };
void CCreatureAction_PerformExpressionExtended::PreventExpression(){ ((CExprField*)(m_holder + 0x78))->Assign(kExprName_00842c50); }
int main(){
    CCreatureAction_PerformExpressionExtended o; char holder[0x100]; o.m_holder=holder;
    o.PreventExpression();
    if(g_dst_00842c50!=(void*)(holder+0x78)||g_name_00842c50!=kExprName_00842c50){std::printf("BAD\n");return 1;}
    std::printf("EXPR842C50_OK\n"); return 0;
}