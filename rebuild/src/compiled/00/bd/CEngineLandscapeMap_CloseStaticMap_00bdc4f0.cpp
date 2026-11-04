#include "candidates/landscape_background_lod_compiled.h"
#include "engine/CLocalDetailCacheMap.h"
void CEngineLandscapeMap::CloseStaticMap() {
    Clear();
    StaticMapFileBlockManager=0;
}
