#include <cstdio>
struct S { void* p; };
static int g_seen=0;
static void __cdecl orf_notify(void* q){ (void)q; g_seen=1; }
static void OnReadFinished(S* self){ if(self->p) orf_notify(self->p); }
int main(){ int mk; S obj; obj.p=&mk; OnReadFinished(&obj);
  S nul; nul.p=0; OnReadFinished(&nul);
  if(!g_seen){ std::printf("004e726b_TEST FAIL\n"); return 1;}
  std::printf("004e726b_TEST PASS\n"); return 0;}