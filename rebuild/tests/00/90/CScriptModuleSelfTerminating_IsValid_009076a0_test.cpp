#include "engine/CCombatSequence_TrollOpeningGroundPound.h"
#include <cstdio>
struct CCombatSequence_TrollOpeningGroundPound; static int __fastcall iv_helper(CCombatSequence_TrollOpeningGroundPound*);
static int g_r=5; static int __fastcall iv_helper(CCombatSequence_TrollOpeningGroundPound*){ return g_r; }
static bool f(CCombatSequence_TrollOpeningGroundPound* self){ return iv_helper(self) > self->f28 ? true : false; }
int main(){ CCombatSequence_TrollOpeningGroundPound o; o.f28=3; g_r=5; if(!f(&o)){ std::printf("009076a0_TEST FAIL\n"); return 1;}
 o.f28=8; if(f(&o)){ std::printf("009076a0_TEST FAIL\n"); return 1;}
 std::printf("009076a0_TEST PASS\n"); return 0;}