#include <cstdio>
struct S; static int g_n=0;
struct S { int x; void M(); };
int __fastcall ss_sub(S* self){ g_n++; return g_n<3 ? 4 : 0; }
void S::M(){ int r; do { r = ss_sub(this); } while(r==4); }
int main(){ S o; o.M(); if(g_n!=3){ std::printf("0091bcf0_TEST FAIL %d\n",g_n); return 1;}
 std::printf("0091bcf0_TEST PASS\n"); return 0;}