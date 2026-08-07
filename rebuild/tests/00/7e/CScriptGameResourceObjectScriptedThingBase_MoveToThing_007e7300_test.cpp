#include <cstdio>
static int g_hit=0;
struct Inner { virtual void d0(){}virtual void d1(){}virtual void d2(){}virtual void d3(){}virtual void d4(){} virtual void V(int a0,int a1,int a2,int a3,int a4,int a5,int a6){ g_hit=1; } };
struct S { char pad[8]; Inner* f8; void MoveToThing(int a0,int a1,int a2,int a3,int a4,int a5,int a6); };
void S::MoveToThing(int a0,int a1,int a2,int a3,int a4,int a5,int a6){ Inner* p = this->f8; if(p) p->V(a0,a1,a2,a3,a4,a5,a6); }
int main(){ Inner inr; S o; o.f8=&inr; o.MoveToThing(1,1,1,1,1,1,1); if(!g_hit){ std::printf("007e7300_TEST FAIL\n"); return 1;}
 S z; z.f8=0; z.MoveToThing(1,1,1,1,1,1,1);
 std::printf("007e7300_TEST PASS\n"); return 0;}