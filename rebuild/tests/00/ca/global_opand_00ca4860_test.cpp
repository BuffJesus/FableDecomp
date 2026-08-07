#include <cstdio>
typedef void (__stdcall* Fn)(void*);
static int g_ran=0; static void __stdcall impl(void* p){ (void)p; g_ran=1; }
static Fn g_pred_fn=impl;
static void* op_and(void* self){ g_pred_fn(self); return self; }
int main(){ int m; if(op_and(&m)!=&m||!g_ran){ std::printf("00ca4860_TEST FAIL\n"); return 1;}
 std::printf("00ca4860_TEST PASS\n"); return 0;}