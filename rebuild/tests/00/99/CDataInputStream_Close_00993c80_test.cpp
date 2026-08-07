#include <cstdio>
struct DS { char pad[4]; int f4; int f8; int fc; int f10; int f14; };
void __fastcall fn_00993c80(DS* self){ self->f4=0; self->f8=0; self->f10=0; self->f14=0; self->fc=0; }
int main(){ DS d; d.f4=d.f8=d.fc=d.f10=d.f14=9; fn_00993c80(&d);
 if(d.f4||d.f8||d.fc||d.f10||d.f14){ printf("00993c80_TEST FAIL\n"); return 1; }
 printf("00993c80_TEST PASS\n"); return 0; }