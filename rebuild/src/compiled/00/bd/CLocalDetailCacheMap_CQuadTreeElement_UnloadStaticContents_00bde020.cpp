#include "engine/CLocalDetailCacheMap.h"
#include "engine/CEngineStaticMapFileBlockManager.h"
void CLocalDetailCacheMap::CQuadTreeElement::UnloadStaticContents(CEngineStaticMapFileBlockManager* manager) {
    if(StateFlags&8) {
        manager->ReleaseRequest(FileBlockPos);
        StateFlags&=~8;
    }
    if(StateFlags&4) {
        RemoveObjectCollections();
        for(unsigned i=0;i<4;++i) {
            delete Children[i];
            Children[i]=0;
        }
        StateFlags&=~4;
    }
}
