#include "rebuild_abi.h"
#include <cstdio>
struct CSpellNode2 { int v; };
static CSpellNode2 g_n_880200; static CSpellNode2 *g_c_880200=0;
extern "C" CSpellNode2 *FABLE_STDCALL FableSpellWrap_880200(void *arg){ g_n_880200.v=(int)(long)arg; return &g_n_880200; }
extern "C" void FABLE_FASTCALL FableSpellConsume_880200(CSpellNode2 *node){ g_c_880200=node; }
struct CSpellContainerList { void AddChild(void *arg); };
void CSpellContainerList::AddChild(void *arg){ FableSpellConsume_880200(FableSpellWrap_880200(arg)); }
int main(){
    CSpellContainerList l; l.AddChild((void*)5);
    if(g_c_880200!=&g_n_880200||g_n_880200.v!=5){std::printf("BAD\n");return 1;}
    std::printf("ADDCHILD880_OK\n"); return 0;
}