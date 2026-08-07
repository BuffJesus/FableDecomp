#include <cstdio>
struct CNavRegion; struct S { char pad[0xF]; unsigned char b0f; void* f10; };
static const CNavRegion* f(S* self){ if(!self->b0f) return (const CNavRegion*)self->f10; return 0; }
int main(){ int m; S o; o.b0f=0; o.f10=&m; if((void*)f(&o)!=&m){ std::printf("00a75c00_TEST FAIL\n"); return 1;}
 o.b0f=1; if(f(&o)!=0){ std::printf("00a75c00_TEST FAIL\n"); return 1;}
 std::printf("00a75c00_TEST PASS\n"); return 0;}