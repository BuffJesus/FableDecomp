#include <cstdio>
struct V2 { int a; int b; };
static V2* CanMove(V2* self, V2* src){ self->a=src->a; self->b=src->b; return self; }
int main(){ V2 s; s.a=0; s.b=0; V2 src; src.a=7; src.b=9;
 V2* r=CanMove(&s,&src);
 if(r!=&s || s.a!=7 || s.b!=9){ std::printf("00643be2_TEST FAIL\n"); return 1;}
 std::printf("00643be2_TEST PASS\n"); return 0;}