#include <cstdio>
typedef void (__stdcall* Fn)(void*, int, int);
static void* g_p=0; static int g_a=-1,g_b=-1;
static void __stdcall impl(void* p, int a, int b){ g_p=p; g_a=a; g_b=b; }
static Fn g_delete_fn=impl;
static void operator_delete(void* p){ g_delete_fn(p, 0, 0x8000); }
int main(){ int m; operator_delete(&m); if(g_p!=&m||g_a!=0||g_b!=0x8000){ std::printf("00a66750_TEST FAIL\n"); return 1;}
 std::printf("00a66750_TEST PASS\n"); return 0;}