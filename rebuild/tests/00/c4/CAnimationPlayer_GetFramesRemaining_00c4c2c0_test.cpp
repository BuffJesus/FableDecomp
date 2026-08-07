#include <cstdio>
struct S { char pad[8]; int a; char pad2[4]; int b; };
static int __stdcall gfr_conv(int x){ return x; }
static int GetFramesRemaining(S* self){ return gfr_conv(self->a - self->b); }
int main(){ S obj; obj.a=30; obj.b=12; if(GetFramesRemaining(&obj)!=18){ std::printf("00c4c2c0_TEST FAIL\n"); return 1;}
 std::printf("00c4c2c0_TEST PASS\n"); return 0;}