#include <cstdio>
struct That2 { void* m(int a); };
struct GB { char pad[0x6b0]; That2* f6b0; };
static That2* gThat; static int gA;
void* That2::m(int a){ gThat=this; gA=a; return (void*)0xDEF; }
static That2 t2i; static GB gbi={ {0}, &t2i };
GB* g_00bf36e0=&gbi;
void* __fastcall fn_00bf36e0(int arg){ GB* g=g_00bf36e0; return g->f6b0->m(arg); }
int main(){ if(fn_00bf36e0(9)!=(void*)0xDEF||gThat!=&t2i||gA!=9){ printf("00bf36e0_TEST FAIL\n"); return 1; }
 printf("00bf36e0_TEST PASS\n"); return 0; }