#include <cstdio>
struct S { char pad[0x188]; int f188; char pad2[0x88]; int f214; };
static void f(S* self){ self->f214=0; self->f188=0; }
int main(){ S o; o.f188=9;o.f214=9; f(&o); if(o.f188||o.f214){ std::printf("005fad30_TEST FAIL\n"); return 1;}
 std::printf("005fad30_TEST PASS\n"); return 0;}