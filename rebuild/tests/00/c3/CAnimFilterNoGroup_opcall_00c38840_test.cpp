#include <cstdio>
struct S { char pad[0xF8]; void* fF8; };
static bool f(S* self, void* e){ (void)e; return self->fF8==0; }
int main(){ S o; o.fF8=0; if(!f(&o,0)){ std::printf("00c38840_TEST FAIL\n"); return 1;}
 int m; o.fF8=&m; if(f(&o,0)){ std::printf("00c38840_TEST FAIL\n"); return 1;}
 std::printf("00c38840_TEST PASS\n"); return 0;}