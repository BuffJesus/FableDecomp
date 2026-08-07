#include <cstdio>
struct S; static int g_hit=0;
struct S { char pad[32]; int fo; void M(); };
void __fastcall cl_h_00bdc4f0(S* self){ g_hit=1; }
void S::M(){ cl_h_00bdc4f0(this); this->fo = 0; }
int main(){ S o; o.fo=9; o.M(); if(g_hit!=1||o.fo!=0){ std::printf("00bdc4f0_TEST FAIL\n"); return 1;}
 std::printf("00bdc4f0_TEST PASS\n"); return 0;}