#include <cstdio>
struct S; static int __fastcall iv_helper(S*);
struct S { char pad[0x28]; int f28; };
static int g_r=5; static int __fastcall iv_helper(S*){ return g_r; }
static bool f(S* self){ return iv_helper(self) > self->f28 ? true : false; }
int main(){ S o; o.f28=3; g_r=5; if(!f(&o)){ std::printf("009076a0_TEST FAIL\n"); return 1;}
 o.f28=8; if(f(&o)){ std::printf("009076a0_TEST FAIL\n"); return 1;}
 std::printf("009076a0_TEST PASS\n"); return 0;}