#include <cstdio>
static int g_hit=0;
struct Inner { virtual void d0(){}virtual void d1(){}virtual void d2(){}virtual void d3(){}virtual void d4(){}virtual void d5(){}virtual void d6(){}virtual void d7(){}virtual void d8(){}virtual void d9(){}virtual void d10(){}virtual void d11(){}virtual void d12(){}virtual void d13(){}virtual void d14(){}virtual void d15(){}virtual void d16(){}virtual void d17(){}virtual void d18(){} virtual void V(int a0,int a1,int a2,int a3,int a4,int a5,int a6){ g_hit=1; } };
struct S { char pad[8]; Inner* f8; void PlayCombatAnimation(int a0,int a1,int a2,int a3,int a4,int a5,int a6); };
void S::PlayCombatAnimation(int a0,int a1,int a2,int a3,int a4,int a5,int a6){ Inner* p = this->f8; if(p) p->V(a0,a1,a2,a3,a4,a5,a6); }
int main(){ Inner inr; S o; o.f8=&inr; o.PlayCombatAnimation(1,1,1,1,1,1,1); if(!g_hit){ std::printf("007e73e0_TEST FAIL\n"); return 1;}
 S z; z.f8=0; z.PlayCombatAnimation(1,1,1,1,1,1,1);
 std::printf("007e73e0_TEST PASS\n"); return 0;}