#include <cstdio>
struct S { char pad[8]; int f8; int fC; };
static void f(S* self){ self->fC=-999; self->f8=-777; }
int main(){ S o; f(&o); if(o.f8!=-777||o.fC!=-999){ std::printf("005db2b0_TEST FAIL\n"); return 1;}
 std::printf("005db2b0_TEST PASS\n"); return 0;}