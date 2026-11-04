#define FABLE_MEMORY_ALLOCATOR_OUT_OF_LINE_SUPPORT
#include "fable_memory_allocator_deallocate.h"
#include "engine/CEngineLandscapeMemoryManager.h"

void CEngineLandscapeMemoryManager::FreeVirtual(void* allocation)
{
    if (allocation)
        CMemoryAllocator::Deallocate(allocation);
}
