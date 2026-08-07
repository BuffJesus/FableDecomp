#include <cstdio>
struct S { char pad[4]; void* f4; void* f8; };
static void* f(S* self){ if(!self->f4) return (void*)-1; return self->f8; }
int main(){ int m,n; S o; o.f4=&m; o.f8=&n; if(f(&o)!=&n){ std::printf("00c62780_TEST FAIL\n"); return 1;}
 o.f4=0; if(f(&o)!=(void*)-1){ std::printf("00c62780_TEST FAIL\n"); return 1;}
 std::printf("00c62780_TEST PASS\n"); return 0;}