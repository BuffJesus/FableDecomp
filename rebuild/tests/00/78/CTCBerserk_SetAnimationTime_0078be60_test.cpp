#include "engine/CTCBerserk.h"
#include <cstdio>
static int g_someint=3;
static void f(CTCBerserk* self, float a){ self->ReleasedAnimationTimer=(float)g_someint*a; }
int main(){ CTCBerserk o; f(&o, 2.0f); if(o.ReleasedAnimationTimer!=6.0f){ std::printf("0078be60_TEST FAIL\n"); return 1;}
 std::printf("0078be60_TEST PASS\n"); return 0;}