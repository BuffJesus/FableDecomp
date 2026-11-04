#include "candidates/landscape_background_lod_compiled.h"
#include "engine/CLocalDetailCacheMap.h"
void CLocalDetailCacheMap::CQuadTreeElement::Clear(CEngineStaticMapFileBlockManager* manager) {
    if(StateFlags&8) {
        manager->ReleaseRequest(FileBlockPos);
        StateFlags&=~8;
    }
    for(unsigned i=0;i<4;++i) {
        delete Children[i];
        Children[i]=0;
    }
    while(ObjectCacheGroupCollections) {
        CObjectCacheGroupCollection* next=ObjectCacheGroupCollections->Next;
        delete ObjectCacheGroupCollections;
        ObjectCacheGroupCollections=next;
    }
    StateFlags&=~7;
    FileBlockPos=0;
    FileBlockSize=0;
    OffsetIntoFileBlock=0;
}
