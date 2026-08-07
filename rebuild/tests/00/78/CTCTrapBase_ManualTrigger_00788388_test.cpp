#include <cstdio>
struct MT { char pad38[0x38]; char f38; char pad58[0x58-0x39]; char f58; void tgt(); };
static int gT; void MT::tgt(){ gT=1; }
void __fastcall fn_00788388(MT* self){ if(self->f58==0) return; if(self->f38!=0) return; self->tgt(); }
int main(){ MT m; gT=0; m.f58=1; m.f38=0; fn_00788388(&m); if(gT!=1){ printf("00788388_TEST FAIL\n"); return 1; }
 gT=0; m.f58=0; fn_00788388(&m); if(gT!=0){ printf("00788388_TEST FAIL\n"); return 1; }
 gT=0; m.f58=1; m.f38=1; fn_00788388(&m); if(gT!=0){ printf("00788388_TEST FAIL\n"); return 1; }
 printf("00788388_TEST PASS\n"); return 0; }