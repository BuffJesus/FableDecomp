#include <cstdio>
void* g_thingmgr;
void __fastcall fn_004bbfd0(void* self){ g_thingmgr = self; }
int main(){ int x; fn_004bbfd0(&x); if(g_thingmgr!=&x){ printf("004bbfd0_TEST FAIL\n"); return 1; }
 printf("004bbfd0_TEST PASS\n"); return 0; }