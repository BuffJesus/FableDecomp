#include <cstdio>
struct Sub; struct S { char pad[4]; Sub* sub; };
static unsigned char __fastcall ewu_probe(Sub* p){ (void)p; return 1; }
static bool EditWorldUpdate(S* self){ return ewu_probe(self->sub) ? true : false; }
int main(){ S o; o.sub=(Sub*)1; if(!EditWorldUpdate(&o)){ std::printf("00751a70_TEST FAIL\n"); return 1;}
 std::printf("00751a70_TEST PASS\n"); return 0;}