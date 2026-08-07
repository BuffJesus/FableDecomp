#include "rebuild_abi.h"
#include <cstdio>
struct GData { char pad[0x5c]; unsigned char m_v; };
static GData g_inst;
GData *g_invData_5d9b30=&g_inst;
static int g_flag=1;
extern "C" char FABLE_CDECL FableCheck_5d9b30(){ return (char)g_flag; }
extern "C" char FABLE_CDECL FableIsBuyable_5d9b30(){ return FableCheck_5d9b30() ? (char)g_invData_5d9b30->m_v : (char)0; }
int main(){ g_inst.m_v=9; g_flag=1;
 if(FableIsBuyable_5d9b30()!=9){std::printf("B1\n");return 1;}
 g_flag=0; if(FableIsBuyable_5d9b30()!=0){std::printf("B2\n");return 1;}
 std::printf("IB_5d9b30_OK\n"); return 0; }