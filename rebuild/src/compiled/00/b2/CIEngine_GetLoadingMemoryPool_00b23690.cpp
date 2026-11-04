// CIEngine::GetLoadingMemoryPool @ 0x00B23690 (Ego_r ?GetLoadingMemoryPool@CIEngine@@UAEPAVCMovableResourceMemoryPool@@XZ; vtable slot of CIEngine):
// the default returns 0.
#include "fable_iengine.h"

CMovableResourceMemoryPool * CIEngine::GetLoadingMemoryPool()
{
    return 0;
}
