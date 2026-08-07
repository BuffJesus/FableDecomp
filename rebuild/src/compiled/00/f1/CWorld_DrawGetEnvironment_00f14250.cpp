#include "rebuild_abi.h"
// CWorld::DrawGetEnvironment @ 0x00f14250
struct CWorld { char pad[0x44]; void *m_env; void *DrawGetEnvironment(); };
void *CWorld::DrawGetEnvironment(){ return m_env; }