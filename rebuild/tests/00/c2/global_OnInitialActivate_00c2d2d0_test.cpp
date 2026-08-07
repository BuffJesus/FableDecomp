#include <cstdio>
struct OA { void h1(int a); void h2(); };
static int gH1,gH2,gA; void OA::h1(int a){ gH1++; gA=a; } void OA::h2(){ gH2++; }
void __fastcall fn_00c2d2d0(OA* self){ self->h1(0); self->h2(); }
int main(){ OA o; gH1=gH2=0; fn_00c2d2d0(&o);
 if(gH1!=1||gH2!=1||gA!=0){ printf("00c2d2d0_TEST FAIL\n"); return 1; }
 printf("00c2d2d0_TEST PASS\n"); return 0; }