#include <cstdio>
struct Sub; struct S { char pad[0x74]; Sub* f74; };
static unsigned int probe(Sub* p){ (void)p; return 0x10; }
static bool f(S* self){ return (probe(self->f74) >> 4) & 1; }
int main(){ S o; o.f74=0; if(!f(&o)){ std::printf("00692880_TEST FAIL\n"); return 1;}
 std::printf("00692880_TEST PASS\n"); return 0;}