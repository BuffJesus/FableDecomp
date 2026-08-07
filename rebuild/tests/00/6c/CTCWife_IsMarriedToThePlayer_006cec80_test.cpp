#include <cstdio>
struct S; static int __fastcall imp_helper(S*);
struct S { char pad[0x20]; int f20; };
static int g_r=5; static int __fastcall imp_helper(S*){ return g_r; }
static bool f(S* self){ return self->f20 <= imp_helper(self) ? true : false; }
int main(){ S o; o.f20=3; g_r=5; if(!f(&o)){ std::printf("006cec80_TEST FAIL\n"); return 1;}
 o.f20=8; if(f(&o)){ std::printf("006cec80_TEST FAIL\n"); return 1;}
 std::printf("006cec80_TEST PASS\n"); return 0;}