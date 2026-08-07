#include "rebuild_abi.h"
#include <cstdio>
struct CHeroSub { int v; };
static int g_a_88df80=0, g_b_88df80=0; static void *g_seen_88df80=0;
struct CDigTarget { int v; void GoDigging(int a, int b){ g_seen_88df80=this; g_a_88df80=a; g_b_88df80=b; } };
static CDigTarget g_t_88df80;
extern "C" CDigTarget *FABLE_FASTCALL FableHeroResolve_88df80(CHeroSub *s){ return &g_t_88df80; }
struct CGameScriptInterface { char pad00[0x14]; CHeroSub *m_hero; void HeroGoDigging(); };
void CGameScriptInterface::HeroGoDigging(){ CDigTarget *t=FableHeroResolve_88df80(m_hero); t->GoDigging(0x1e,0); }
int main(){
    CGameScriptInterface g; CHeroSub h; g.m_hero=&h; g.HeroGoDigging();
    if(g_seen_88df80!=&g_t_88df80||g_a_88df80!=0x1e||g_b_88df80!=0){std::printf("BAD\n");return 1;}
    std::printf("DIGGING_OK\n"); return 0;
}