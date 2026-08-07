#include <cstdio>
static void* g1=0,*g2=0; static void* g_glob=(void*)0x88;
static void __fastcall dv_h1(void* p){ g1=p; }
static void __fastcall dv_h2(void* p){ g2=p; }
static void Dest_val(void* self){ dv_h1(self); dv_h2(g_glob); }
int main(){ int m; Dest_val(&m); if(g1!=&m||g2!=(void*)0x88){ std::printf("0055d4c0_TEST FAIL\n"); return 1;}
 std::printf("0055d4c0_TEST PASS\n"); return 0;}