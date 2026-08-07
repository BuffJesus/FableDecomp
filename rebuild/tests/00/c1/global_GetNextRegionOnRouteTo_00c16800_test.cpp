#include <cstdio>
struct RG { char pad[4]; int f4; int h(int a); };
static int gA; int RG::h(int a){ gA=a; return 99; }
int __fastcall fn_00c16800(RG* self){ if(!self->f4) return 0; return self->h(0); }
int main(){ RG r; r.f4=1; if(fn_00c16800(&r)!=99||gA!=0){ printf("00c16800_TEST FAIL\n"); return 1; }
 r.f4=0; if(fn_00c16800(&r)!=0){ printf("00c16800_TEST FAIL\n"); return 1; }
 printf("00c16800_TEST PASS\n"); return 0; }