#include <cstdio>
struct Inner { virtual void d0(){}virtual void d1(){}virtual void d2(){}virtual void d3(){}virtual void d4(){}virtual void d5(){}virtual void d6(){}virtual void d7(){}virtual void d8(){}virtual void d9(){}virtual void d10(){}virtual void d11(){}virtual void d12(){}virtual void d13(){}virtual void d14(){} virtual bool tgt(){ return true; } };
struct S { char pad[4]; Inner* f4; };
static bool f(S* self){ Inner* p=self->f4; if(!p) return false; return p->tgt(); }
int main(){ S o; o.f4=0; if(f(&o)!=(false)){ std::printf("004aaab0_TEST FAIL\n"); return 1;}
 Inner inr; o.f4=&inr; if(f(&o)!=true){ std::printf("004aaab0_TEST FAIL\n"); return 1;}
 std::printf("004aaab0_TEST PASS\n"); return 0;}