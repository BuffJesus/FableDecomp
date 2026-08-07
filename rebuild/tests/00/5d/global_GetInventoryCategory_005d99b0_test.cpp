#include "rebuild_abi.h"
#include <cstdio>
struct GData { char pad[0x64]; int m_v; };
static GData g_inst;
GData *g_invData_5d99b0=&g_inst;
static int g_flag=1;
extern "C" char FABLE_CDECL FableCheck_5d99b0(){ return (char)g_flag; }
extern "C" int FABLE_CDECL FableGetInv_5d99b0(){ return FableCheck_5d99b0() ? g_invData_5d99b0->m_v : 0; }
int main(){ g_inst.m_v=55; g_flag=1;
 if(FableGetInv_5d99b0()!=55){std::printf("B1\n");return 1;}
 g_flag=0; if(FableGetInv_5d99b0()!=0){std::printf("B2\n");return 1;}
 std::printf("GI_5d99b0_OK\n"); return 0; }