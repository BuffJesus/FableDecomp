#include "rebuild_abi.h"
#include <cstdio>
struct Res { char pad[8]; int m_v; };
static Res g_res;
struct Sub74 { void Get(Res **out); };
void Sub74::Get(Res **out){ g_res.m_v=88; *out=&g_res; }
struct CCombatActionBase { char pad[0x74]; Sub74 *m_sub; int GetAnimationTransitionInTime(); };
int CCombatActionBase::GetAnimationTransitionInTime(){ Res *r; m_sub->Get(&r); return r->m_v; }
int main(){ Sub74 s; CCombatActionBase o; o.m_sub=&s;
 if(o.GetAnimationTransitionInTime()!=88){std::printf("B1\n");return 1;}
 std::printf("GA_8571a0_OK\n"); return 0; }