#include "rebuild_abi.h"
#include <cstdio>
static int g_a=0,g_b=99;
void FABLE_STDCALL Imp_cb0e90(void *a, int b){ g_a=(a!=0); g_b=b; }
void (FABLE_STDCALL *g_import_cb0e90)(void *a, int b)=Imp_cb0e90;
struct CWorldMap { char pad4[4]; void *m_h; char pad[4]; int m_result; int PrepareForLoad(); };
int CWorldMap::PrepareForLoad(){ g_import_cb0e90(m_h,-1); return m_result; }
int main(){ CWorldMap o; int x; o.m_h=&x; o.m_result=42;
 if(o.PrepareForLoad()!=42||g_b!=-1){std::printf("B1\n");return 1;}
 std::printf("PF_cb0e90_OK\n"); return 0; }