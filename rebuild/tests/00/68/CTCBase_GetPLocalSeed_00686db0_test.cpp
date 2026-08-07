#include <cstdio>
static char g_blk[0x40];
static char* g_localseed_base = g_blk;
static unsigned long* GetPLocalSeed(){ return reinterpret_cast<unsigned long*>(*reinterpret_cast<char**>(g_localseed_base+0x1c)+0x16140); }
int main(){ char tgt; char* pt=&tgt; *reinterpret_cast<char**>(g_blk+0x1c)=pt;
 unsigned long* r=GetPLocalSeed();
 if((char*)r != pt+0x16140){ std::printf("00686db0_TEST FAIL\n"); return 1;}
 std::printf("00686db0_TEST PASS\n"); return 0;}