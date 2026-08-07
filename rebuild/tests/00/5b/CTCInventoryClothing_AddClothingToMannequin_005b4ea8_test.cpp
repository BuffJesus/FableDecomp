#include <cstdio>
struct IC { char pad[0x158]; int f158; };
static int gF,gA;
int __stdcall h_005b4ea8(int arg, int field){ gF=field; gA=arg; return 55; }
int __fastcall fn_005b4ea8(IC* self, int _edx, int arg){ return h_005b4ea8(arg, self->f158); }
int main(){ IC ic; ic.f158=99; if(fn_005b4ea8(&ic,0,42)!=55||gF!=99||gA!=42){ printf("005b4ea8_TEST FAIL\n"); return 1; }
 printf("005b4ea8_TEST PASS\n"); return 0; }