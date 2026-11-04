#include "engine/CLocalDetailCacheMap.h"
#include "engine/CEngineStaticMapFileBlockManager.h"
void CLocalDetailCacheMap::OnMapVisibilityChanged(bool visible) {
    if(!visible) {
        QuadTreeElement.UnloadStaticContents(StaticMapFileBlockManager);
        if(StaticMapFileBlockManager) StaticMapFileBlockManager->DeleteUnusedBlocks();
    }
}
