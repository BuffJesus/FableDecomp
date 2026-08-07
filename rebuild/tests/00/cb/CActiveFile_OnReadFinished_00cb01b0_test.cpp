#include <cstdio>
typedef void (__stdcall* Fn)(void*);
static int g_seen=0; static void __stdcall impl(void* p){ (void)p; g_seen=1; }
static Fn g_orf_fn=impl;
struct S { void* p; };
static void OnReadFinished(S* self){ if(self->p) g_orf_fn(self->p); }
int main(){ int m; S o; o.p=&m; OnReadFinished(&o); S n; n.p=0; OnReadFinished(&n);
 if(!g_seen){ std::printf("00cb01b0_TEST FAIL\n"); return 1;}
 std::printf("00cb01b0_TEST PASS\n"); return 0;}