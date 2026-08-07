#include <cstdio>
static void* g_p=0; static int g_n=-1;
static void __stdcall bil_helper(void* p, int n){ g_p=p; g_n=n; }
struct CMainGameComponent { void BeginInputLoading(void* a); };
void CMainGameComponent::BeginInputLoading(void* a){ bil_helper(a, 2); }
int main(){ int m; CMainGameComponent o; o.BeginInputLoading(&m); if(g_p!=&m||g_n!=2){ std::printf("00c31b70_TEST FAIL\n"); return 1;}
 std::printf("00c31b70_TEST PASS\n"); return 0;}