#include "rebuild_abi.h"
#include <cstdio>
static int g_n=0;
struct Sub { void Cancel(int n); };
void Sub::Cancel(int n){ g_n=n; }
struct CTCMultiStrike { char pad4; Sub *m_sub; char pad[0x14-8]; char m_active; void CancelForCutscene(); };
void CTCMultiStrike::CancelForCutscene(){ if(!m_active) return; m_sub->Cancel(0x87); }
int main(){ Sub s; CTCMultiStrike o; o.m_sub=&s; o.m_active=1; o.CancelForCutscene();
 if(g_n!=0x87){std::printf("B1\n");return 1;}
 o.m_active=0; g_n=0; o.CancelForCutscene(); if(g_n!=0){std::printf("B2\n");return 1;}
 std::printf("CF_7cc9c0_OK\n"); return 0; }