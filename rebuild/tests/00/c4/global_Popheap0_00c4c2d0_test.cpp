#include <cstdio>
struct PH { char pad[4]; int f4; char pad2[0x10-0x8]; int f10; int f14; int h(int s); };
static int gS; int PH::h(int s){ gS=s; return s+1; }
int __fastcall fn_00c4c2d0(PH* self){ return self->h(self->f10 + self->f14 + self->f4); }
int main(){ PH p; p.f4=1; p.f10=2; p.f14=3; if(fn_00c4c2d0(&p)!=7||gS!=6){ printf("00c4c2d0_TEST FAIL\n"); return 1; }
 printf("00c4c2d0_TEST PASS\n"); return 0; }