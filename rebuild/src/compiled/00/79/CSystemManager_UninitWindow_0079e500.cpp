#include "rebuild_abi.h"
// CSystemManager::UninitWindow @ 0x0079e500
extern "C" void FABLE_STDCALL FableUninit_79e500(void *h);
struct CSystemManager { char pad[0x40]; void *m_handle; char pad2[0x46-0x44]; char m_done; void UninitWindow(); };
void CSystemManager::UninitWindow(){
    if (m_done) return;
    void *h = m_handle;
    m_done = 1;
    FableUninit_79e500(h);
}