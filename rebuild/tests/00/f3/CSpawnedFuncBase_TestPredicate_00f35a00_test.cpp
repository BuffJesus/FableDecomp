#include <cstdio>
struct P { virtual bool v0(){ return false; } };
struct S { char pad[0x2c]; P* f2c; };
static bool f(S* self){ P* p=self->f2c; if(!p) return true; return p->v0(); }
int main(){ S o; o.f2c=0; if(!f(&o)){ std::printf("00f35a00_TEST FAIL\n"); return 1;}
 P p; o.f2c=&p; if(f(&o)){ std::printf("00f35a00_TEST FAIL\n"); return 1;}
 std::printf("00f35a00_TEST PASS\n"); return 0;}