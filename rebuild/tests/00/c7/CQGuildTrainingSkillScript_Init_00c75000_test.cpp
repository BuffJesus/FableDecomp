#include <cstdio>
struct S { char pad[8]; int f8; int fc; char pad2[8]; int f18; int f1c; };
static void f(S* self){ self->f8=0; self->fc=0; self->f18=0; self->f1c=0; }
int main(){ S o; o.f8=o.fc=o.f18=o.f1c=9; f(&o); if(o.f8||o.fc||o.f18||o.f1c){ std::printf("00c75000_TEST FAIL\n"); return 1;}
 std::printf("00c75000_TEST PASS\n"); return 0;}