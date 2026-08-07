#include <cstdio>
static int g=0;
struct Inner { virtual void d0(){} virtual void d1(){} virtual void d2(){} virtual void d3(){}
               virtual void d4(){} virtual void d5(){} virtual void d6(){} virtual void d7(){}
               virtual void d8(){} virtual void d9(){} virtual void d10(){} virtual void d11(){}
               virtual void d12(){} virtual void d13(){} virtual void d14(){} virtual void d15(){}
               virtual void V(){ g=1; } };
struct S { char pad[0x98]; Inner* f98; };
static void f(S* self){ Inner* p=self->f98; if(p) p->V(); }
int main(){ S z; z.f98=0; f(&z); Inner inr; S o; o.f98=&inr; f(&o);
 if(!g){ std::printf("006978c0_TEST FAIL\n"); return 1;}
 std::printf("006978c0_TEST PASS\n"); return 0;}