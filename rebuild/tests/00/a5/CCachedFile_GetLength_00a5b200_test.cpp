#include <cstdio>
struct Inner { virtual void d0(){} virtual void d1(){} virtual void d2(){} virtual void d3(){}
               virtual void d4(){} virtual void d5(){} virtual void d6(){} virtual void d7(){} virtual void d8(){}
               virtual int V9(){ return 42; } };
struct S { char pad[0x1c]; Inner* f1c; };
static int f(S* self){ Inner* p=self->f1c; if(!p) return 0; return p->V9(); }
int main(){ S z; z.f1c=0; if(f(&z)!=0){ std::printf("00a5b200_TEST FAIL\n"); return 1;}
 Inner inr; S o; o.f1c=&inr; if(f(&o)!=42){ std::printf("00a5b200_TEST FAIL\n"); return 1;}
 std::printf("00a5b200_TEST PASS\n"); return 0;}