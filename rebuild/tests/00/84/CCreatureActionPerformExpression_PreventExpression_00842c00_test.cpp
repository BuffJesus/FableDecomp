#include "rebuild_abi.h"
#include <cstdio>
const char kExprName_00842c00[] = "expr";
static void *g_dst_00842c00=0; static const char *g_name_00842c00=0;
struct CExprField { void Assign(const char *name){ g_dst_00842c00=this; g_name_00842c00=name; } };
struct CCreatureAction_PerformExpression { char pad00[0xa8]; char *m_holder; void PreventExpression(); };
void CCreatureAction_PerformExpression::PreventExpression(){ ((CExprField*)(m_holder + 0x78))->Assign(kExprName_00842c00); }
int main(){
    CCreatureAction_PerformExpression o; char holder[0x100]; o.m_holder=holder;
    o.PreventExpression();
    if(g_dst_00842c00!=(void*)(holder+0x78)||g_name_00842c00!=kExprName_00842c00){std::printf("BAD\n");return 1;}
    std::printf("EXPR842C00_OK\n"); return 0;
}