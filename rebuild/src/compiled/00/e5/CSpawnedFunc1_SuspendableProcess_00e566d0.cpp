#include "rebuild_abi.h"
// CSpawnedFunc1<...>::SuspendableProcess @ 0x00e566d0
// push esi; mov esi,ecx; mov eax,[esi+0x3c]; mov ecx,[esi+0x38]; push eax; call [esi+0x34]; mov byte[esi+5],1; pop esi; ret
// Invoke the bound member (this->f34) on target this->f38 with arg this->f3c, then flag started.
struct CSpawnTarget { void Invoke(void *arg); };
typedef void (CSpawnTarget::*SpawnPMF)(void*);
struct CSpawnedFunc1 {
    char pad00[5];
    unsigned char m_started;      // +0x5
    char pad06[0x34-6];
    SpawnPMF m_fn;                // +0x34
    CSpawnTarget *m_target;       // +0x38
    void *m_arg;                  // +0x3c
    void SuspendableProcess();
};
void CSpawnedFunc1::SuspendableProcess()
{
    (m_target->*m_fn)(m_arg);
    m_started = 1;
}