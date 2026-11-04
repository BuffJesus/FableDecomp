#include <new>
#include "engine/CGPUScratchBufferTextureManager.h"
#include "fable_stlport_vector_impl.h"

void CGPUScratchBufferTextureManager::FreeMemoryPage(long page)
{
    MemoryPages[page].ScratchPool=-1;
    while(MemoryPages[page].ResourceList.size()>0)
        FreeResourceEntry(MemoryPages[page].ResourceList[0]);
    FreeTexturePageSlots.push_back(page);
}
