#include "rebuild_abi.h"
#include <cstdio>
struct CSpellNode { int v; };
static CSpellNode g_node_6a4dd0; static CSpellNode *g_consumed_6a4dd0=0;
extern "C" CSpellNode *FABLE_STDCALL FableSpellWrap_6a4dd0(void *arg){ g_node_6a4dd0.v=(int)(long)arg; return &g_node_6a4dd0; }
extern "C" void FABLE_FASTCALL FableSpellConsume_6a4dd0(CSpellNode *node){ g_consumed_6a4dd0=node; }
struct CSpellContainerList { void AddChild(void *arg); };
void CSpellContainerList::AddChild(void *arg){ FableSpellConsume_6a4dd0(FableSpellWrap_6a4dd0(arg)); }
int main(){
    CSpellContainerList l; l.AddChild((void*)5);
    if(g_consumed_6a4dd0!=&g_node_6a4dd0||g_node_6a4dd0.v!=5){std::printf("BAD\n");return 1;}
    std::printf("ADDCHILD6A_OK\n"); return 0;
}