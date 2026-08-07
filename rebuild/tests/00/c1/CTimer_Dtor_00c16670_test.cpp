#include <cstdio>
struct T;
struct Mgr { virtual void d0(){} virtual void Unreg(T* p){ g_seen=(void*)p; } static void* g_seen; };
void* Mgr::g_seen=0;
static Mgr g_m; static Mgr* g_mgr=&g_m;
static void Dtor(T* self){ g_mgr->Unreg(self); }
int main(){ T* x=(T*)0x1234; Dtor(x); if(Mgr::g_seen!=(void*)x){ std::printf("00c16670_TEST FAIL\n"); return 1;}
 std::printf("00c16670_TEST PASS\n"); return 0;}