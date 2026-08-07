#include <cstdio>
int g_a=11; int g_b=0;
int* AddChildPrimitive(){ g_b = g_a; return &g_a; }
int main(){ int* r=AddChildPrimitive();
 if(r!=&g_a||g_b!=11){ std::printf("00a6cbb0_TEST FAIL\n"); return 1;}
 std::printf("00a6cbb0_TEST PASS\n"); return 0;}