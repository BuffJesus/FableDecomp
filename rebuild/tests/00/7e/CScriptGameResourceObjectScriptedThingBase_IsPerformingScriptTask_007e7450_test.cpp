#include <cstdio>
struct Inner { virtual void d0(){}virtual void d1(){}virtual void d2(){}virtual void d3(){}virtual void d4(){}virtual void d5(){}virtual void d6(){}virtual void d7(){}virtual void d8(){}virtual void d9(){}virtual void d10(){}virtual void d11(){}virtual void d12(){}virtual void d13(){}virtual void d14(){}virtual void d15(){}virtual void d16(){}virtual void d17(){}virtual void d18(){}virtual void d19(){}virtual void d20(){}virtual void d21(){}virtual void d22(){}virtual void d23(){}virtual void d24(){}virtual void d25(){} virtual bool V(){ return true; } };
struct S { char pad[8]; Inner* f8; };
static bool f(S* self){ Inner* p=self->f8; if(!p) return false; return p->V(); }
int main(){ S z; z.f8=0; if(f(&z)){ std::printf("007e7450_TEST FAIL\n"); return 1;}
 Inner inr; S o; o.f8=&inr; if(!f(&o)){ std::printf("007e7450_TEST FAIL\n"); return 1;}
 std::printf("007e7450_TEST PASS\n"); return 0;}