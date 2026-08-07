#include <cstdio>
struct GS { char pad[0x134]; char f134; char virtual_tgt(); };
char GS::virtual_tgt(){ return 9; }
char __fastcall fn_006f5700(GS* self){ if(self->f134) return self->virtual_tgt(); return 0; }
int main(){ GS g; g.f134=1; if(fn_006f5700(&g)!=9){ printf("006f5700_TEST FAIL\n"); return 1; }
 g.f134=0; if(fn_006f5700(&g)!=0){ printf("006f5700_TEST FAIL\n"); return 1; }
 printf("006f5700_TEST PASS\n"); return 0; }