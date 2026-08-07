#include <cstdio>
struct DF { char pad[0x3c]; int f3c; };
void __fastcall fn_00643fe4(DF* self, int _edx, int v, char b){ self->f3c = b ? v : 0; }
int main(){ DF d; d.f3c=-1; fn_00643fe4(&d,0,9,1); if(d.f3c!=9){ printf("00643fe4_TEST FAIL\n"); return 1; }
 fn_00643fe4(&d,0,9,0); if(d.f3c!=0){ printf("00643fe4_TEST FAIL\n"); return 1; }
 printf("00643fe4_TEST PASS\n"); return 0; }