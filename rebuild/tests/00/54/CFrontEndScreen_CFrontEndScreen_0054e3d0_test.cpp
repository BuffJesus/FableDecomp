#include <cstdio>
#include <cstring>
extern "C" void* __fastcall CFrontEndScreen_CFrontEndScreen_0054e3d0(void*,void*,long);
extern "C" void* const FableFrontEndScreenVtable[]={(void*)1};
extern "C" void* const FableFrontEndScreenInterface4[]={(void*)2};
extern "C" void* const FableFrontEndScreenInterface18[]={(void*)3};
static void*g_baseObject;static long g_baseArg;static int g_baseCalls;
extern "C" void __fastcall FableFrontEndScreenConstructBase(void*o,void*,long a){g_baseObject=o;g_baseArg=a;++g_baseCalls;}
int main(){unsigned char o[0x170];std::memset(o,0xcd,sizeof(o));g_baseCalls=0;void*r=CFrontEndScreen_CFrontEndScreen_0054e3d0(o,0,0x12345678);bool p=r==o&&g_baseCalls==1&&g_baseObject==o&&g_baseArg==0x12345678&&*(void**)(o)==FableFrontEndScreenVtable&&*(void**)(o+4)==FableFrontEndScreenInterface4&&*(void**)(o+0x18)==FableFrontEndScreenInterface18&&*(unsigned long*)(o+0x160)==0&&*(unsigned long*)(o+0x164)==0&&*(unsigned long*)(o+0x168)==0;if(!p){std::printf("FSE2_0054e3d0_TEST FAIL ret=%d base=%d arg=%d vt=%d tails=%d%d%d\n",r==o,g_baseCalls,g_baseArg==0x12345678,*(void**)o==FableFrontEndScreenVtable,*(unsigned long*)(o+0x160)==0,*(unsigned long*)(o+0x164)==0,*(unsigned long*)(o+0x168)==0);return 1;}std::puts("FSE2_0054e3d0_TEST PASS");return 0;}
