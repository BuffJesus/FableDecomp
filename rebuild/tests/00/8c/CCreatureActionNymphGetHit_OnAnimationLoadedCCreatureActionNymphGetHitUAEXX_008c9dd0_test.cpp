#include "rebuild_abi.h"
#include <cstdio>
static int g_base=0;
struct C { char pad[0x10]; int m_a; int m_b; void OnAnimationLoaded(); };
extern "C" void FABLE_FASTCALL FableBase_8c9dd0(C *self){ g_base=1; }
void C::OnAnimationLoaded(){ FableBase_8c9dd0(this); m_b=m_a/2; }
int main(){ C o; o.m_a=10; o.m_b=0; o.OnAnimationLoaded();
 if(!g_base||o.m_b!=5){std::printf("B1\n");return 1;}
 std::printf("OA_8c9dd0_OK\n"); return 0; }