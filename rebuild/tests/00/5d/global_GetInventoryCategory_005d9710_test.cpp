#include "rebuild_abi.h"
#include <cstdio>
struct GData { char pad[0x4c]; int m_v; };
static GData g_inst;
GData *g_invData_5d9710=&g_inst;
static int g_flag=1;
extern "C" char FABLE_CDECL FableCheck_5d9710(){ return (char)g_flag; }
extern "C" int FABLE_CDECL FableGetInv_5d9710(){ return FableCheck_5d9710() ? g_invData_5d9710->m_v : 0; }
int main(){ g_inst.m_v=55; g_flag=1;
 if(FableGetInv_5d9710()!=55){std::printf("B1\n");return 1;}
 g_flag=0; if(FableGetInv_5d9710()!=0){std::printf("B2\n");return 1;}
 std::printf("GI_5d9710_OK\n"); return 0; }