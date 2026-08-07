#include "rebuild_abi.h"
// CAIBrain::GetCurrentStateGroupScriptPriority @ 0x0088b760
struct State { char pad[8]; int m_prio; char pad2[0x10-0xc]; char m_flag; };
struct CAIBrain { char pad[0x38]; State *m_state; int GetPrio(); };
int CAIBrain::GetPrio(){
    State *s = m_state;
    return (s && !s->m_flag) ? s->m_prio : 0;
}