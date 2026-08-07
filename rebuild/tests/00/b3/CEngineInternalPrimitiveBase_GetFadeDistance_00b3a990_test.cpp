#include <cstdio>
struct Pair { int a; int b; };
struct FD { char pad[0x14]; int f14; int f18; };
Pair* __fastcall fn_00b3a990(FD* self, int _edx, Pair* out){ out->a=self->f14; out->b=self->f18; return out; }
int main(){ FD f; f.f14=11; f.f18=22; Pair p; Pair* r=fn_00b3a990(&f,0,&p);
 if(r!=&p||p.a!=11||p.b!=22){ printf("00b3a990_TEST FAIL\n"); return 1; }
 printf("00b3a990_TEST PASS\n"); return 0; }