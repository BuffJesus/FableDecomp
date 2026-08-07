#include "rebuild_abi.h"
// CAIBrain::GetCurrentStateGroupDebugText @ 0x00c628d0
// mov eax,[ecx+4]; test; jne L; or eax,-1; ret; L: mov ecx,[ecx+4]; mov eax,[ecx]; jmp [eax+N]
// If no active state group, return -1; else tail-call a virtual on it.
struct CStateGroup {
    virtual int v0(); virtual int v1(); virtual int v2(); virtual int v3(); virtual int v4();
};
struct CAIBrain { void *pad0; CStateGroup *m_group; int GetCurrentStateGroupDebugText(); };
int CAIBrain::GetCurrentStateGroupDebugText()
{
    if (m_group == 0) return -1;
    return m_group->v4();
}