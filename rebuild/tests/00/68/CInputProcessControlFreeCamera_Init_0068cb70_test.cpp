#include <cstdio>
static char g_f=0;
struct S { void Init(); };
void S::Init(){ if(!g_f) g_f=1; }
int main(){ S o; g_f=0; o.Init(); if(g_f!=1){ std::printf("0068cb70_TEST FAIL\n"); return 1;}
 o.Init(); if(g_f!=1){ std::printf("0068cb70_TEST FAIL\n"); return 1;}
 std::printf("0068cb70_TEST PASS\n"); return 0;}