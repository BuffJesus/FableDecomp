#include <cstdio>
static int g_hit=0;
struct Inner { virtual void d0(){}virtual void d1(){} virtual void V(int a0){ g_hit=1; } };
struct S { char pad[8]; Inner* f8; void SummonerLightningOrbAttackTarget(int a0); };
void S::SummonerLightningOrbAttackTarget(int a0){ Inner* p = this->f8; if(p) p->V(a0); }
int main(){ Inner inr; S o; o.f8=&inr; o.SummonerLightningOrbAttackTarget(1); if(!g_hit){ std::printf("007e72d0_TEST FAIL\n"); return 1;}
 S z; z.f8=0; z.SummonerLightningOrbAttackTarget(1);
 std::printf("007e72d0_TEST PASS\n"); return 0;}