#include "engine/CEngineWorldMap.h"
#include "engine/CEngineMap.h"
#include "candidates/landscape_background_lod_compiled.h"
void CEngineWorldMap::OnGameMapUnloaded(long id) {
    CEngineMap* map=MapList[id];
    if(map->LandscapeMap) map->LandscapeMap->OnGameMapUnloaded();
    map->GameMap=0;
}
