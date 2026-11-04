#include <new>
#include "engine/CGPUScratchBufferTextureManager.h"
#include "fable_stlport_vector_impl.h"

void CGPUScratchBufferTextureManager::FreeResourceEntry(long index)
{
    FreeTextureSlots.push_back(index);
    long page=Resources[index].PageIndex;
    Resources[index].ResourceType=RESOURCE_TYPE_NOT_ALLOCATED;
    delete Resources[index].Texture;
    delete Resources[index].Surface;
    Resources[index].Texture=0;
    Resources[index].Surface=0;
    CArrayVector<long>& list=MemoryPages[page].ResourceList;
    for(unsigned long i=0;i<list.size();++i) {
        if(list[i]==index) {
            list.erase(list.begin()+i);
            break;
        }
    }
}
