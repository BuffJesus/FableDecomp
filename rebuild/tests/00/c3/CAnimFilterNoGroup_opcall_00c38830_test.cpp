#include <cstdio>
struct S { char pad[0xF8]; int fF8; };
static bool f(S* self, void* e){ (void)e; return self->fF8==6; }
int main(){ S o; o.fF8=6; if(!f(&o,0)){ std::printf("00c38830_TEST FAIL\n"); return 1;}
 o.fF8=5; if(f(&o,0)){ std::printf("00c38830_TEST FAIL\n"); return 1;}
 std::printf("00c38830_TEST PASS\n"); return 0;}