#include <cstdio>
struct S; struct S { char pad[0x44]; unsigned short w44; };
static int g_ran=0; static void __fastcall glp_helper(S* p){ (void)p; g_ran=1; }
static unsigned short f(S* self){ glp_helper(self); return self->w44; }
int main(){ S o; o.w44=0xBEEF; if(f(&o)!=0xBEEF||!g_ran){ std::printf("0083f640_TEST FAIL\n"); return 1;}
 std::printf("0083f640_TEST PASS\n"); return 0;}