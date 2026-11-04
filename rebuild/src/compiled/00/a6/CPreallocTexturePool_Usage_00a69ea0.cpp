#include "engine/CPreallocTexturePool.h"
#include "fable_managed_texture_data.h"
unsigned long CPreallocTexturePool::GetTexturesAllocated() {
    unsigned long allocated=0;
    for(unsigned long i=0;i<Count;++i)
        if(Textures[i]->Allocated) ++allocated;
    return allocated;
}
unsigned long CPreallocTexturePool::GetTexturesInUse(unsigned long unusedFrames) {
    unsigned long used=0;
    for(unsigned long i=0;i<Count;++i)
        if(Textures[i]->Allocated && Textures[i]->GetUnusedFrames()<=unusedFrames) ++used;
    return used;
}
