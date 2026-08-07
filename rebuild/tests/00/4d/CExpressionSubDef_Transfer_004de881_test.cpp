#include <cstdio>
static void* g_sub=0;
struct CPersistContext { void Xfer(void* sub); };
void CPersistContext::Xfer(void* sub){ g_sub=sub; }
struct S { char pad[0x28]; void Transfer(CPersistContext* ctx); };
void S::Transfer(CPersistContext* ctx){ ctx->Xfer(reinterpret_cast<char*>(this) + 0x28); }
int main(){ S o; CPersistContext c; o.Transfer(&c); if(g_sub!=(char*)&o+0x28){ std::printf("004de881_TEST FAIL\n"); return 1;}
 std::printf("004de881_TEST PASS\n"); return 0;}