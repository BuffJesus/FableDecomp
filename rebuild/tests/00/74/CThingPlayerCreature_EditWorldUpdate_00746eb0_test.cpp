#include <cstdio>
struct Sub; struct S { char pad[4]; Sub* f4; };
static unsigned char ewu_probe(Sub* p){ (void)p; return 0; }
static bool f(S* self){ return ewu_probe(self->f4) ? false : true; }
int main(){ S o; o.f4=0; if(!f(&o)){ std::printf("00746eb0_TEST FAIL\n"); return 1;}
 std::printf("00746eb0_TEST PASS\n"); return 0;}