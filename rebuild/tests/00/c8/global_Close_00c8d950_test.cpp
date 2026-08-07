#include <cstdio>
struct DS { char pad[8]; int f8; int fc; int f10; int f14; int f18; };
void __fastcall fn_00c8d950(DS* self){ self->f8=0; self->fc=0; self->f10=0; self->f14=0; self->f18=0; }
int main(){ DS d; d.f8=d.fc=d.f10=d.f14=d.f18=9; fn_00c8d950(&d);
 if(d.f8||d.fc||d.f10||d.f14||d.f18){ printf("00c8d950_TEST FAIL\n"); return 1; }
 printf("00c8d950_TEST PASS\n"); return 0; }