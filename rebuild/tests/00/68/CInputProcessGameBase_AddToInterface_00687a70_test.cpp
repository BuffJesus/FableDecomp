#include <cstdio>
static void* g_m=0;
struct CInputProcessManager;
struct S { char pad[0x24]; void* f24; bool AddToInterface(CInputProcessManager* m); bool Helper(CInputProcessManager* m); };
bool S::Helper(CInputProcessManager* m){ g_m=m; return true; }
bool S::AddToInterface(CInputProcessManager* m){ this->f24 = m; return this->Helper(m); }
int main(){ int x; S o; bool r=o.AddToInterface((CInputProcessManager*)&x);
 if(!r||o.f24!=&x||g_m!=&x){ std::printf("00687a70_TEST FAIL\n"); return 1;}
 std::printf("00687a70_TEST PASS\n"); return 0;}