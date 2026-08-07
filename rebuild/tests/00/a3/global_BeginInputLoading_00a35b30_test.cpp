#include <cstdio>
struct N { void h(int a, int b); };
struct BL { char pad[8]; N* f8; };
static int gA,gB; static N* gS; void N::h(int a, int b){ gS=this; gA=a; gB=b; }
void __fastcall fn_00a35b30(BL* self, int _edx, int arg){ self->f8->h(arg, 0); }
int main(){ N n; BL b; b.f8=&n; fn_00a35b30(&b,0,42);
 if(gS!=&n||gA!=42||gB!=0){ printf("00a35b30_TEST FAIL\n"); return 1; }
 printf("00a35b30_TEST PASS\n"); return 0; }