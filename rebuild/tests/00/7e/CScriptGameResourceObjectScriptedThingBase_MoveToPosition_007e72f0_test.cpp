#include <cstdio>
static int g_hit=0;
struct Inner { virtual void d0(){}virtual void d1(){}virtual void d2(){}virtual void d3(){} virtual void V(int a0,int a1,int a2,int a3,int a4){ g_hit=1; } };
struct S { char pad[8]; Inner* f8; void MoveToPosition(int a0,int a1,int a2,int a3,int a4); };
void S::MoveToPosition(int a0,int a1,int a2,int a3,int a4){ Inner* p = this->f8; if(p) p->V(a0,a1,a2,a3,a4); }
int main(){ Inner inr; S o; o.f8=&inr; o.MoveToPosition(1,1,1,1,1); if(!g_hit){ std::printf("007e72f0_TEST FAIL\n"); return 1;}
 S z; z.f8=0; z.MoveToPosition(1,1,1,1,1);
 std::printf("007e72f0_TEST PASS\n"); return 0;}