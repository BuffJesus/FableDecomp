#include <cstdio>
struct S;
static S* g_p=0;static int g_hit=0;
struct S { int x; void M(); };
S* __fastcall od1(S* self){ return g_p; }
void __fastcall od2(S* p){ g_hit=1; }
void S::M(){ S* p=od1(this); if(p) od2(p); }
int main(){ S o; g_p=&o; o.M(); if(g_hit!=1){ std::printf("00799120_TEST FAIL\n"); return 1;}
 g_p=0; g_hit=0; o.M(); if(g_hit!=0){ std::printf("00799120_TEST FAIL\n"); return 1;}
 std::printf("00799120_TEST PASS\n"); return 0;}