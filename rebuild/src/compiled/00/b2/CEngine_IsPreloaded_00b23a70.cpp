#include "rebuild_abi.h"
// CEngine::IsPreloaded @ 0x00b23a70
struct CEngine { char pad18[0x18]; char m_b18; char pad19[0xb]; char m_b24; bool IsPreloaded(); };
bool CEngine::IsPreloaded()
{
    if (m_b18 || m_b24) return false;
    return true;
}