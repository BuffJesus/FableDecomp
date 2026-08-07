#include <cstdio>
struct S { char pad[4]; void* f4; char pad2[4]; void* f0c; };
static void* f(S* self){ if(!self->f4) return 0; return self->f0c; }
int main(){ int m,n; S o; o.f4=&m; o.f0c=&n; if(f(&o)!=&n){ std::printf("00c628c0_TEST FAIL\n"); return 1;}
 o.f4=0; if(f(&o)!=0){ std::printf("00c628c0_TEST FAIL\n"); return 1;}
 std::printf("00c628c0_TEST PASS\n"); return 0;}