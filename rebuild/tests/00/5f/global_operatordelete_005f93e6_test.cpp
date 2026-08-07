#include <cstdio>
static void* g_p=0; static int g_a=-1,g_b=-1;
static void __stdcall del_helper(void* p, int a, int b){ g_p=p; g_a=a; g_b=b; }
static void __stdcall operator_delete(void* p){ del_helper(p, 0, 0); }
int main(){ int m; operator_delete(&m); if(g_p!=&m||g_a||g_b){ std::printf("005f93e6_TEST FAIL\n"); return 1;}
 std::printf("005f93e6_TEST PASS\n"); return 0;}