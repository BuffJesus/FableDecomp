#include <cstdio>
static void* g_sub=0;
struct CPersistContext { void Xfer(void* sub); };
void CPersistContext::Xfer(void* sub){ g_sub=sub; }
struct S { char pad[0x28]; void AddLearnedExpression(CPersistContext* ctx); };
void S::AddLearnedExpression(CPersistContext* ctx){ ctx->Xfer(reinterpret_cast<char*>(this) + 0x28); }
int main(){ S o; CPersistContext c; o.AddLearnedExpression(&c); if(g_sub!=(char*)&o+0x28){ std::printf("0047e186_TEST FAIL\n"); return 1;}
 std::printf("0047e186_TEST PASS\n"); return 0;}