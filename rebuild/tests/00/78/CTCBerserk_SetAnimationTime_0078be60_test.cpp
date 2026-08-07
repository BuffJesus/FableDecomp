#include <cstdio>
static int g_someint=3;
struct S { char pad[0x1c]; float f1c; };
static void f(S* self, float a){ self->f1c=(float)g_someint*a; }
int main(){ S o; f(&o, 2.0f); if(o.f1c!=6.0f){ std::printf("0078be60_TEST FAIL\n"); return 1;}
 std::printf("0078be60_TEST PASS\n"); return 0;}