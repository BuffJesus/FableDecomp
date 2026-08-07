#include <cstdio>
static void* g_p=(void*)1; static int g_z=-1;
static void __stdcall bil_helper(void* p, int z){ g_p=p; g_z=z; }
struct CMainGameComponent { void BeginInputLoading(void* a); };
void CMainGameComponent::BeginInputLoading(void* a){ bil_helper(a, 0); }
int main(){ int m; CMainGameComponent o; o.BeginInputLoading(&m);
 if(g_p!=&m||g_z!=0){ std::printf("00a12890_TEST FAIL\n"); return 1;}
 std::printf("00a12890_TEST PASS\n"); return 0;}