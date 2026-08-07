#include <cstdio>
struct Pool { void* Alloc(unsigned s); };
static unsigned g_s=0; static void* g_ret=(void*)0x77;
void* Pool::Alloc(unsigned s){ g_s=s; return g_ret; }
static void* __stdcall operator_new(unsigned size, Pool* pool){ return pool->Alloc(size); }
int main(){ Pool p; void* r=operator_new(32, &p); if(g_s!=32||r!=(void*)0x77){ std::printf("0048e062_TEST FAIL\n"); return 1;}
 std::printf("0048e062_TEST PASS\n"); return 0;}