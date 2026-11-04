#include "candidates/landscape_background_lod_compiled.h"
#include "engine/CLocalDetailCacheMap.h"
void CLocalDetailCacheMap::CloseStaticMap() {
    QuadTreeElement.Clear(StaticMapFileBlockManager);
    StaticMapFileBlockManager=0;
}
