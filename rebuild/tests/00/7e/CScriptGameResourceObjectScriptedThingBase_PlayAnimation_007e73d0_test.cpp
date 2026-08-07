#include <cstdio>
static int g_hit=0;
struct Inner { virtual void d0(){}virtual void d1(){}virtual void d2(){}virtual void d3(){}virtual void d4(){}virtual void d5(){}virtual void d6(){}virtual void d7(){}virtual void d8(){}virtual void d9(){}virtual void d10(){}virtual void d11(){}virtual void d12(){}virtual void d13(){}virtual void d14(){}virtual void d15(){}virtual void d16(){}virtual void d17(){} virtual void V(int a0,int a1,int a2,int a3,int a4,int a5,int a6,int a7){ g_hit=1; } };
struct S { char pad[8]; Inner* f8; void PlayAnimation(int a0,int a1,int a2,int a3,int a4,int a5,int a6,int a7); };
void S::PlayAnimation(int a0,int a1,int a2,int a3,int a4,int a5,int a6,int a7){ Inner* p = this->f8; if(p) p->V(a0,a1,a2,a3,a4,a5,a6,a7); }
int main(){ Inner inr; S o; o.f8=&inr; o.PlayAnimation(1,1,1,1,1,1,1,1); if(!g_hit){ std::printf("007e73d0_TEST FAIL\n"); return 1;}
 S z; z.f8=0; z.PlayAnimation(1,1,1,1,1,1,1,1);
 std::printf("007e73d0_TEST PASS\n"); return 0;}