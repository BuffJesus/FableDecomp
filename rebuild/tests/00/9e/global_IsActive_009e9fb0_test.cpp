#include <cstdio>
struct GI2 { void** vtbl; };
static char __fastcall slot2(GI2* s){ return 7; }
static void* vt2[8]; static GI2 inst2={vt2}; GI2* g_9e9fb0=&inst2;
typedef char (__fastcall *pf2)(GI2*);
char __fastcall fn_009e9fb0(){ GI2* g=g_9e9fb0; if(g) return ((pf2)g->vtbl[0x18/4])(g); return 0; }
int main(){ vt2[0x18/4]=(void*)&slot2; if(fn_009e9fb0()!=7){ printf("009e9fb0_TEST FAIL\n"); return 1; }
 g_9e9fb0=0; if(fn_009e9fb0()!=0){ printf("009e9fb0_TEST FAIL\n"); return 1; }
 printf("009e9fb0_TEST PASS\n"); return 0; }