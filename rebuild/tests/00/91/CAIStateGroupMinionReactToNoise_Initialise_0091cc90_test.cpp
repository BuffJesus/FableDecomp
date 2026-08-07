#include "rebuild_abi.h"
#include <cstdio>
struct CAIStateGroup_MinionReactToNoise;
static int g_baseInit_91cc90=0;
extern "C" void FABLE_FASTCALL FableReactBaseInit_91cc90(CAIStateGroup_MinionReactToNoise *self){ g_baseInit_91cc90=1; }
struct CAIStateGroup_MinionReactToNoise { char pad00[0x20]; unsigned char m_f20; unsigned char m_f21; char pad22[2]; long m_f24; void Initialise(); };
void CAIStateGroup_MinionReactToNoise::Initialise(){ m_f21=0; m_f20=0; m_f24=(long)0xffffff88; FableReactBaseInit_91cc90(this); }
int main(){
    CAIStateGroup_MinionReactToNoise s; s.m_f20=1; s.m_f21=1; s.m_f24=0; s.Initialise();
    if(s.m_f20!=0||s.m_f21!=0||s.m_f24!=(long)0xffffff88||!g_baseInit_91cc90){std::printf("BAD\n");return 1;}
    std::printf("REACTINIT_OK\n"); return 0;
}