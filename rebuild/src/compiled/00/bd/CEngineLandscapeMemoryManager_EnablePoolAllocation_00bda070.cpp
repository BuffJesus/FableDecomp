#include "engine/CEngineLandscapeMemoryManager.h"

void CEngineLandscapeMemoryManager::EnablePoolAllocation(bool enabled)
{
    if (PoolAllocationEnabled!=enabled) {
        FreeDisposalList();
        PoolAllocationEnabled=enabled;
    }
}
