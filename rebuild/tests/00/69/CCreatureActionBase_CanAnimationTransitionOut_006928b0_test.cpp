#include <cstdio>
struct Sub; struct S { char pad[0x74]; Sub* sub; };
static int cat_probe(Sub* p){ (void)p; return 2; }
static bool CanAnimationTransitionOut(S* self){ return (~cat_probe(self->sub)) & 1; }
int main(){ S obj; obj.sub=0; if(!CanAnimationTransitionOut(&obj)){ std::printf("006928b0_TEST FAIL\n"); return 1;}
 std::printf("006928b0_TEST PASS\n"); return 0;}