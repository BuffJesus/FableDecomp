#include "engine/CEngineMap.h"
#include "candidates/landscape_background_lod_compiled.h"
void CEngineMap::OnGameMapUnloaded() {
    if(LandscapeMap) LandscapeMap->OnGameMapUnloaded();
    GameMap=0;
}
