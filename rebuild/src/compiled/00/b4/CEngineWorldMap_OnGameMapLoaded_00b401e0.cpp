#include "engine/CEngineWorldMap.h"
#include "engine/CEngineMap.h"
#include "engine/CIDrawWorldMap.h"
#include "fable_engine_internal.h"
#include "candidates/landscape_background_lod_compiled.h"
void CEngineWorldMap::OnGameMapLoaded(long id) {
    CEngineMap* map=MapList[id];
    CDrawMapInfo info=NEngineInternal::GPEngineWorldMap->GameWorldMap->DrawGetMapInfo(id);
    map->GameMap=info.PMap;
    map->LandscapeMap->OnGameMapLoaded();
}
