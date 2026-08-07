#include <cstdio>
static void* g_p=0; static int g_a=0,g_b=0;
struct P { int gnr(int a0, int a1); };
int P::gnr(int a0, int a1){ g_p=this; g_a=a0; g_b=a1; return 5; }
struct S { char pad[0x44]; P* f44; int M(int a0, int a1); };
int S::M(int a0, int a1){ P* p=this->f44; if(p) return p->gnr(a0, a1); return -1; }
int main(){ char x; S o; o.f44=(P*)&x; if(o.M(2,3)!=5||g_a!=2||g_b!=3){ std::printf("004fb4d0_TEST FAIL\n"); return 1;}
 o.f44=0; if(o.M(2,3)!=-1){ std::printf("004fb4d0_TEST FAIL\n"); return 1;}
 std::printf("004fb4d0_TEST PASS\n"); return 0;}