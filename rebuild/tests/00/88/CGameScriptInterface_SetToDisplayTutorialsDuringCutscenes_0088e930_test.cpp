#include <cstdio>
struct GS2 { char pad[0xcc]; char fcc; };
static GS2 g_gs;
GS2* __cdecl get_88e930(){ return &g_gs; }
void __fastcall fn_0088e930(void* self, int _edx, char arg){ get_88e930()->fcc = arg; }
int main(){ fn_0088e930(0,0,(char)5); if(g_gs.fcc!=5){ printf("0088e930_TEST FAIL\n"); return 1; }
 printf("0088e930_TEST PASS\n"); return 0; }