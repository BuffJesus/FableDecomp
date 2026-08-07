#include <cstdio>
struct S { char pad[0x1d4]; void* f1d4; };
static bool f(S* self){ return self->f1d4!=0; }
int main(){ int m; S o; o.f1d4=&m; if(!f(&o)){ std::printf("00c3eba0_TEST FAIL\n"); return 1;}
 o.f1d4=0; if(f(&o)){ std::printf("00c3eba0_TEST FAIL\n"); return 1;}
 std::printf("00c3eba0_TEST PASS\n"); return 0;}