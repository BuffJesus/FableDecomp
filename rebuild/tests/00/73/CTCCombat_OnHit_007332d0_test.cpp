#include <cstdio>
int g_7332d0=123;
struct OH { char pad[0x1c]; int f1c; char pad2[0x50-0x20]; int f50; };
void __fastcall fn_007332d0(OH* self, int _edx, int arg){ self->f1c = g_7332d0; self->f50 = -1; }
int main(){ OH o; fn_007332d0(&o,0,0); if(o.f1c!=123||o.f50!=-1){ printf("007332d0_TEST FAIL\n"); return 1; }
 printf("007332d0_TEST PASS\n"); return 0; }