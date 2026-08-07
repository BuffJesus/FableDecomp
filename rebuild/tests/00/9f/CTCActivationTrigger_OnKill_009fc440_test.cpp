#include <cstdio>
struct S;
static int g_hit=0;
struct P { void M(S* self); };
struct S { char pad[8]; P* f8; };
void P::M(S* self){ g_hit=1; }
void __fastcall OnKill(S* self){ P* p=self->f8; if(p) p->M(self); }
int main(){ P pp; S o; o.f8=&pp; OnKill(&o);
 if(g_hit!=1){ std::printf("009fc440_TEST FAIL\n"); return 1;}
 g_hit=0; o.f8=0; OnKill(&o);
 if(g_hit!=0){ std::printf("009fc440_TEST FAIL\n"); return 1;}
 std::printf("009fc440_TEST PASS\n"); return 0;}