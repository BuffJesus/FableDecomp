#include <cstdio>
struct S { char pad[8]; int f8; int fC; };
static void f(S* self){ self->f8=2; self->fC=0; }
int main(){ S o; o.f8=9;o.fC=9; f(&o); if(o.f8!=2||o.fC!=0){ std::printf("005db250_TEST FAIL\n"); return 1;}
 std::printf("005db250_TEST PASS\n"); return 0;}