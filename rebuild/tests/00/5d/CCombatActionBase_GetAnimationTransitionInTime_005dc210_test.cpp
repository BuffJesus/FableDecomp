#include <cstdio>
struct AT { char pad[8]; int f8; };
static AT g_at;
AT* __stdcall h_005dc210(AT** out){ g_at.f8=123; *out=&g_at; return &g_at; }
int __fastcall fn_005dc210(void* self){ AT* p; h_005dc210(&p); return p->f8; }
int main(){ if(fn_005dc210(0)!=123){ printf("005dc210_TEST FAIL\n"); return 1; }
 printf("005dc210_TEST PASS\n"); return 0; }