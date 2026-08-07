#include "rebuild_abi.h"
// CTCMultiStrike::CancelForCutscene @ 0x007cc9c0
struct Sub { void Cancel(int n); };
struct CTCMultiStrike { char pad4; Sub *m_sub; char pad[0x14-8]; char m_active; void CancelForCutscene(); };
void CTCMultiStrike::CancelForCutscene(){
    if (!m_active) return;
    m_sub->Cancel(0x87);
}