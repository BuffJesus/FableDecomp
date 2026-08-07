#include <cstdio>
static void* g_sub=0;
struct CPersistContext { void Xfer(void* sub); };
void CPersistContext::Xfer(void* sub){ g_sub=sub; }
struct S { char pad[0x25]; void Transfer(CPersistContext* ctx); };
void S::Transfer(CPersistContext* ctx){ ctx->Xfer(reinterpret_cast<char*>(this) + 0x25); }
int main(){ S o; CPersistContext c; o.Transfer(&c); if(g_sub!=(char*)&o+0x25){ std::printf("004ddb56_TEST FAIL\n"); return 1;}
 std::printf("004ddb56_TEST PASS\n"); return 0;}