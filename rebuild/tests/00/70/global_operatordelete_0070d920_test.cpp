#include <cstdio>
static int g_hit=0;
int __stdcall gd_sub(void* p, int a, int b){ g_hit=a+b+(p?1:0); return 0; }
void __stdcall op_del(void* p){ gd_sub(p,1,0); }
int main(){ int x; op_del(&x);
 if(g_hit!=2){ std::printf("0070d920_TEST FAIL %d\n",g_hit); return 1;}
 std::printf("0070d920_TEST PASS\n"); return 0;}