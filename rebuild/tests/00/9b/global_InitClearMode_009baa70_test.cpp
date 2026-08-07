#include <cstdio>
struct IC { int f0; char pad[0x11]; char f15; char pad2[2]; int f18; };
void __fastcall fn_009baa70(IC* self){ self->f18=4; self->f0=0; self->f15=0; }
int main(){ IC c; c.f0=9;c.f15=9;c.f18=9; fn_009baa70(&c);
 if(c.f18!=4||c.f0!=0||c.f15!=0){ printf("009baa70_TEST FAIL\n"); return 1; }
 printf("009baa70_TEST PASS\n"); return 0; }