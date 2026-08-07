#include "rebuild_abi.h"
#include <cstdio>
struct CSubA { int v; }; struct CSubB { int w; };
static char g_store_890670[0x40]; void *g_singleton_890670 = g_store_890670;
static CSubB g_b_890670;
extern "C" CSubB *FABLE_FASTCALL FableProjH1_890670(CSubA *a){ g_b_890670.w = a ? 1 : 0; return &g_b_890670; }
extern "C" bool FABLE_FASTCALL FableProjH2_890670(CSubB *b){ return b->w != 0; }
extern "C" bool FABLE_FASTCALL FableIsHeroInProjectile_890670(){ CSubA *a=*(CSubA**)((char*)g_singleton_890670+0x1c); return FableProjH2_890670(FableProjH1_890670(a)); }
int main(){
    CSubA a; *(CSubA**)((char*)g_singleton_890670+0x1c)=&a;
    if(!FableIsHeroInProjectile_890670()){std::printf("BAD\n");return 1;}
    std::printf("HEROPROJ_OK\n"); return 0;
}