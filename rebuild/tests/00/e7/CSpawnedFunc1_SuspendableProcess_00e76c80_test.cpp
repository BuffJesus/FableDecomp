#include "rebuild_abi.h"
#include <cstdio>
static void *g_seen_00e76c80 = 0;
struct CSpawnTarget { void Invoke(void *arg){ g_seen_00e76c80 = arg; } };
typedef void (CSpawnTarget::*SpawnPMF)(void*);
struct CSpawnedFunc1 { char pad00[5]; unsigned char m_started; char pad06[0x34-6]; SpawnPMF m_fn; CSpawnTarget *m_target; void *m_arg; void SuspendableProcess(); };
void CSpawnedFunc1::SuspendableProcess(){ (m_target->*m_fn)(m_arg); m_started=1; }
int main(){
    CSpawnedFunc1 s; s.m_started=0; CSpawnTarget tgt;
    s.m_fn=&CSpawnTarget::Invoke; s.m_target=&tgt; s.m_arg=(void*)0x1234;
    s.SuspendableProcess();
    if(s.m_started!=1||g_seen_00e76c80!=(void*)0x1234){std::printf("BAD\n");return 1;}
    std::printf("SUSPEND6C80_OK\n"); return 0;
}