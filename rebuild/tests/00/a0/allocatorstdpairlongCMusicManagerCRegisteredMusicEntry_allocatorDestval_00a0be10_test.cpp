#include <cstdio>
struct S; static int g1=0,g2=0; static void* g2a=0;
struct S { char pad[0xc]; int f0c; void M(); };
void __fastcall dv_h1(S* p){ g1=1; }
void __fastcall dv_h2(void* p){ g2=1; g2a=p; }
void S::M(){ dv_h1(this); dv_h2((char*)this + 0xc); }
int main(){ S o; o.M(); if(g1!=1||g2!=1||g2a!=((char*)&o+0xc)){ std::printf("00a0be10_TEST FAIL\n"); return 1;}
 std::printf("00a0be10_TEST PASS\n"); return 0;}