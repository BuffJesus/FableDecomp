#include "rebuild_abi.h"
// CScriptModuleSelfTerminate::IsFinished @ 0x00c49620
// push esi; esi=ecx; lea ecx,[esi+0x1c]; call helper; ecx=[esi+0x3c]; sub ecx,eax; eax=ecx; ret
struct Sub1c;
extern "C" long FABLE_FASTCALL FableCount_c49620(Sub1c *s);
struct CScriptModuleSelfTerminate { char pad1c[0x1c]; char m_sub[0x20]; long m_target; long IsFinished(); };
long CScriptModuleSelfTerminate::IsFinished()
{
    return m_target - FableCount_c49620((Sub1c*)&m_sub[0]);
}