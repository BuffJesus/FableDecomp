#include <cstdio>
static void* g_sub=0;
struct CPersistContext { void Xfer(void* sub); };
void CPersistContext::Xfer(void* sub){ g_sub=sub; }
struct S { char pad[0x3c]; void Transfer(CPersistContext* ctx); };
void S::Transfer(CPersistContext* ctx){ ctx->Xfer(reinterpret_cast<char*>(this) + 0x3c); }
int main(){ S o; CPersistContext c; o.Transfer(&c); if(g_sub!=(char*)&o+0x3c){ std::printf("0046c294_TEST FAIL\n"); return 1;}
 std::printf("0046c294_TEST PASS\n"); return 0;}