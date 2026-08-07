#include <cstdio>
struct Inner { virtual void d0(){}virtual void d1(){}virtual void d2(){}virtual void d3(){}virtual void d4(){}virtual void d5(){}virtual void d6(){}virtual void d7(){}virtual void d8(){}virtual void d9(){}virtual void d10(){} virtual int tgt(){ return 42; } };
struct S { char pad[4]; Inner* f4; };
static int f(S* self){ Inner* p=self->f4; if(!p) return 0; return p->tgt(); }
int main(){ S o; o.f4=0; if(f(&o)!=(0)){ std::printf("004aaa60_TEST FAIL\n"); return 1;}
 Inner inr; o.f4=&inr; if(f(&o)!=42){ std::printf("004aaa60_TEST FAIL\n"); return 1;}
 std::printf("004aaa60_TEST PASS\n"); return 0;}