#include <cstdio>
struct S { char pad[8]; int v; };
static int isf_count(S* self){ (void)self; return 100; }
static int IsFinished(S* self){ return isf_count(self) + self->v; }
int main(){ S obj; obj.v=23; if(IsFinished(&obj)!=123){ std::printf("00c1d810_TEST FAIL\n"); return 1;}
 std::printf("00c1d810_TEST PASS\n"); return 0;}