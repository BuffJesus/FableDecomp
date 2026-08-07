#include "rebuild_abi.h"
#include <cstdio>
static int g_called=0; static void *g_h=0;
extern "C" void FABLE_STDCALL FableUninit_79e500(void *h){ g_called++; g_h=h; }
struct CSystemManager { char pad[0x40]; void *m_handle; char pad2[0x46-0x44]; char m_done; void UninitWindow(); };
void CSystemManager::UninitWindow(){ if(m_done) return; void *h=m_handle; m_done=1; FableUninit_79e500(h); }
int main(){ CSystemManager o; int x; o.m_handle=&x; o.m_done=0;
 o.UninitWindow(); if(g_called!=1||o.m_done!=1||g_h!=&x){std::printf("B1\n");return 1;}
 o.UninitWindow(); if(g_called!=1){std::printf("B2\n");return 1;}
 std::printf("UW_79e500_OK\n"); return 0; }