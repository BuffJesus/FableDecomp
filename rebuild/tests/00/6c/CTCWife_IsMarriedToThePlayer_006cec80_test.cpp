#include "engine/CTCWife.h"
#include <cstdio>
struct CTCWife; static int __fastcall imp_helper(CTCWife*);
static int g_r=5; static int __fastcall imp_helper(CTCWife*){ return g_r; }
static bool f(CTCWife* self){ return self->FrameGotMarriedToThePlayer <= imp_helper(self) ? true : false; }
int main(){ CTCWife o; o.FrameGotMarriedToThePlayer=3; g_r=5; if(!f(&o)){ std::printf("006cec80_TEST FAIL\n"); return 1;}
 o.FrameGotMarriedToThePlayer=8; if(f(&o)){ std::printf("006cec80_TEST FAIL\n"); return 1;}
 std::printf("006cec80_TEST PASS\n"); return 0;}